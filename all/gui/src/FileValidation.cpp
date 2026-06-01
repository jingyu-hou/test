#include "FileValidation.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

QStringList extensionWhitelist()
{
    QStringList list;
    list << "inp" << "frd" << "dat" << "txt" << "iges" << "igs"
         << "step" << "stp" << "brep" << "stl" << "wgs";
    return list;
}

FileValidationResult validateInputFile(const QString &filePath,
                                       const QStringList &allowedExtensions)
{
    QFileInfo fi(filePath);
    if (!fi.exists()) {
        return FileValidationResult::fail(
            QString("File does not exist: %1").arg(filePath));
    }
    if (!fi.isFile()) {
        return FileValidationResult::fail(
            QString("Path is not a file: %1").arg(filePath));
    }
    QString suffix = fi.suffix().toLower();
    if (suffix.isEmpty()) {
        return FileValidationResult::fail(
            QString("File has no extension: %1").arg(filePath));
    }
    if (!allowedExtensions.isEmpty() && !allowedExtensions.contains(suffix)) {
        return FileValidationResult::fail(
            QString("Unsupported file format: .%1. Allowed: %2")
                .arg(suffix, allowedExtensions.join(", ")));
    }

    if (suffix == "inp")  return FileValidationResult::ok(FK_INP);
    if (suffix == "frd")  return FileValidationResult::ok(FK_FRD);
    if (suffix == "dat")  return FileValidationResult::ok(FK_DAT);
    if (suffix == "txt")  return FileValidationResult::ok(FK_TXT_CURVE);
    if (suffix == "iges" || suffix == "igs") return FileValidationResult::ok(FK_IGES);
    if (suffix == "step" || suffix == "stp") return FileValidationResult::ok(FK_STEP);
    if (suffix == "stl")  return FileValidationResult::ok(FK_STL);
    if (suffix == "brep") return FileValidationResult::ok(FK_BREP);
    if (suffix == "wgs")  return FileValidationResult::ok(FK_WGS);
    return FileValidationResult::ok(FK_UNKNOWN);
}

static bool looksLikeInpNodeLine(const QString &line)
{
    QStringList fields = line.split(",", QString::SkipEmptyParts);
    if (fields.size() < 3) return false;
    bool ok = false;
    fields.at(0).toInt(&ok);
    if (!ok) return false;
    fields.at(1).toDouble(&ok);
    if (!ok) return false;
    fields.at(2).toDouble(&ok);
    return ok;
}

static bool looksLikeInpElementLine(const QString &line)
{
    QStringList fields = line.split(",", QString::SkipEmptyParts);
    if (fields.size() < 4) return false;
    bool ok = false;
    fields.at(0).toInt(&ok);
    if (!ok) return false;
    for (int i = 1; i < fields.size(); ++i) {
        fields.at(i).toInt(&ok);
        if (!ok) return false;
    }
    return true;
}

FileValidationResult validateInpFile(const QString &filePath)
{
    QStringList ext; ext << "inp";
    FileValidationResult base = validateInputFile(filePath, ext);
    if (!base.valid) return base;

    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly)) {
        return FileValidationResult::fail(
            QString("Cannot open INP file: %1").arg(filePath));
    }
    if (f.size() == 0) {
        f.close();
        return FileValidationResult::fail(
            QString("INP file is empty: %1").arg(filePath));
    }

    QTextStream ts(&f);
    bool foundNode = false;
    bool foundElement = false;
    bool inNodeSection = false;
    bool inElementSection = false;
    bool hasValidNode = false;
    bool hasValidElement = false;

    while (!ts.atEnd()) {
        QString line = ts.readLine().simplified();
        if (line.isEmpty()) continue;
        if (line.startsWith("**")) continue;
        QString lower = line.toLower();
        if (lower.startsWith("*node")) {
            foundNode = true;
            inNodeSection = true;
            inElementSection = false;
            continue;
        }
        if (lower.startsWith("*element")) {
            foundElement = true;
            inElementSection = true;
            inNodeSection = false;
            continue;
        }
        if (lower.startsWith("*") && !lower.startsWith("*node") && !lower.startsWith("*element")) {
            inNodeSection = false;
            inElementSection = false;
            continue;
        }
        if (inNodeSection && !hasValidNode && looksLikeInpNodeLine(line)) {
            hasValidNode = true;
        }
        if (inElementSection && !hasValidElement && looksLikeInpElementLine(line)) {
            hasValidElement = true;
        }
        if (hasValidNode && hasValidElement) break;
    }
    f.close();

    if (!foundNode) {
        return FileValidationResult::fail(
            QString("Invalid INP file: missing *NODE section. File: %1").arg(filePath));
    }
    if (!foundElement) {
        return FileValidationResult::fail(
            QString("Invalid INP file: missing *ELEMENT section. File: %1").arg(filePath));
    }
    if (!hasValidNode) {
        return FileValidationResult::fail(
            QString("Invalid INP file: no valid node records. File: %1").arg(filePath));
    }
    if (!hasValidElement) {
        return FileValidationResult::fail(
            QString("Invalid INP file: no valid element records. File: %1").arg(filePath));
    }
    return FileValidationResult::ok(FK_INP);
}

FileValidationResult validateFrdFile(const QString &filePath)
{
    QStringList ext; ext << "frd";
    FileValidationResult base = validateInputFile(filePath, ext);
    if (!base.valid) return base;

    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly)) {
        return FileValidationResult::fail(
            QString("Cannot open FRD file: %1").arg(filePath));
    }
    if (f.size() == 0) {
        f.close();
        return FileValidationResult::fail(
            QString("FRD file is empty: %1").arg(filePath));
    }

    QTextStream ts(&f);
    bool found1C = false;
    bool found2C = false;
    bool found3C = false;
    bool hasNodeRecord = false;
    bool hasElementHeader = false;
    int lineCount = 0;
    const int maxLines = 50000;

    while (!ts.atEnd() && lineCount < maxLines) {
        QString line = ts.readLine().simplified();
        ++lineCount;
        if (line.isEmpty()) continue;
        QString id = line.left(2);
        if (id == "1C") {
            found1C = true;
        } else if (id == "2C" && found1C) {
            found2C = true;
        } else if (id == "3C" && found2C) {
            found3C = true;
        } else if (id == "-1" && found2C && !found3C) {
            QStringList fields = line.split(" ", QString::SkipEmptyParts);
            if (fields.size() >= 4) hasNodeRecord = true;
        } else if (id == "-1" && found3C) {
            hasElementHeader = true;
        }
        if (found1C && found2C && found3C && hasNodeRecord && hasElementHeader) break;
    }
    f.close();

    if (!found1C) {
        return FileValidationResult::fail(
            QString("Invalid FRD file: missing 1C header. File: %1").arg(filePath));
    }
    if (!found2C) {
        return FileValidationResult::fail(
            QString("Invalid FRD file: missing 2C node block. File: %1").arg(filePath));
    }
    if (!hasNodeRecord) {
        return FileValidationResult::fail(
            QString("Invalid FRD file: no valid node records. File: %1").arg(filePath));
    }
    if (!found3C) {
        return FileValidationResult::fail(
            QString("Invalid FRD file: missing 3C element block. File: %1").arg(filePath));
    }
    if (!hasElementHeader) {
        return FileValidationResult::fail(
            QString("Invalid FRD file: no valid element records. File: %1").arg(filePath));
    }
    return FileValidationResult::ok(FK_FRD);
}

FileValidationResult validateDatFile(const QString &filePath)
{
    QStringList ext; ext << "dat";
    FileValidationResult base = validateInputFile(filePath, ext);
    if (!base.valid) return base;

    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly)) {
        return FileValidationResult::fail(
            QString("Cannot open DAT file: %1").arg(filePath));
    }
    f.close();
    return FileValidationResult::ok(FK_DAT);
}

FileValidationResult validateTxtCurveFile(const QString &filePath)
{
    QStringList ext; ext << "txt";
    FileValidationResult base = validateInputFile(filePath, ext);
    if (!base.valid) return base;

    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly)) {
        return FileValidationResult::fail(
            QString("Cannot open TXT file: %1").arg(filePath));
    }
    QTextStream ts(&f);
    QString firstLine = ts.readLine();
    f.close();
    if (firstLine.isEmpty()) {
        return FileValidationResult::fail(
            QString("Curve TXT file is empty: %1").arg(filePath));
    }
    return FileValidationResult::ok(FK_TXT_CURVE);
}
