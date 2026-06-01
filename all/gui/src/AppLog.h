#ifndef APPLOG_H
#define APPLOG_H

#include <QString>
#include <QtGlobal>

class AppLog
{
public:
    static void Init();
    static QString LogPath();
    static void Write(const QString &category, const QString &message);
    static void Action(const QString &message);
    static void Dialog(const QString &message);
    static void File(const QString &message);
    static void Error(const QString &message);
    static void QtMessage(QtMsgType type, const char *msg);

private:
    static QString FallbackLogPath();
    static void RotateIfNeeded(const QString &path);
};

#endif // APPLOG_H
