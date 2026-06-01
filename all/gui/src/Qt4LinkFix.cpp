// Force instantiation of operator< for QString (missing from Qt4 shared library)
#include <QString>
bool QStringLessThan(const QString &a, const QString &b) {
    return a < b;
}
