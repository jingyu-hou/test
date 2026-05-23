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
    QTextStream ts(&f);
    QString firstLine = ts.readLine();
    f.close();
    if (firstLine.isEmpty()) {
        return FileValidationResult::fail(
            QString("INP file is empty: %1").arg(filePath));
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
    QTextStream ts(&f);
    QString firstLine = ts.readLine().simplified();
    f.close();
    if (!firstLine.startsWith("1C")) {
        return FileValidationResult::fail(
            QString("File does not appear to be a valid FRD file (missing 1C header): %1")
                .arg(filePath));
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
