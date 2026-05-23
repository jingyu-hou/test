#ifndef FILEVALIDATION_H
#define FILEVALIDATION_H

#include <QString>
#include <QStringList>

enum FileKind {
    FK_UNKNOWN,
    FK_INP,
    FK_FRD,
    FK_DAT,
    FK_TXT_CURVE,
    FK_STEP,
    FK_IGES,
    FK_STL,
    FK_BREP,
    FK_WGS
};

struct FileValidationResult {
    bool valid;
    FileKind kind;
    QString errorMessage;

    FileValidationResult() : valid(false), kind(FK_UNKNOWN) {}
    static FileValidationResult ok(FileKind k) {
        FileValidationResult r;
        r.valid = true;
        r.kind = k;
        return r;
    }
    static FileValidationResult fail(const QString &msg) {
        FileValidationResult r;
        r.valid = false;
        r.kind = FK_UNKNOWN;
        r.errorMessage = msg;
        return r;
    }
};

FileValidationResult validateInputFile(const QString &filePath,
                                       const QStringList &allowedExtensions);
FileValidationResult validateInpFile(const QString &filePath);
FileValidationResult validateFrdFile(const QString &filePath);
FileValidationResult validateDatFile(const QString &filePath);
FileValidationResult validateTxtCurveFile(const QString &filePath);
QStringList extensionWhitelist();

#endif
