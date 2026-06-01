#include "AppLog.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

static bool g_appLogWriting = false;
static const qint64 kMaxLogBytes = 5 * 1024 * 1024;

void AppLog::Init()
{
    Write("APP", "startup");
}

QString AppLog::LogPath()
{
    QString dir = QCoreApplication::applicationDirPath();
    if (dir.isEmpty()) {
        dir = QDir::currentPath();
    }
    QDir appDir(dir);
    if (appDir.mkpath("logs")) {
        const QString path = appDir.filePath("logs/weicme_user_actions.log");
        QFile test(path);
        if (test.open(QIODevice::Append | QIODevice::Text)) {
            test.close();
            return path;
        }
    }
    return FallbackLogPath();
}

QString AppLog::FallbackLogPath()
{
    QDir cwd(QDir::currentPath());
    if (cwd.mkpath("logs")) {
        return cwd.filePath("logs/weicme_user_actions.log");
    }
    return cwd.filePath("weicme_user_actions.log");
}

void AppLog::RotateIfNeeded(const QString &path)
{
    QFileInfo info(path);
    if (!info.exists() || info.size() <= kMaxLogBytes) {
        return;
    }
    const QString oldPath = path + ".1";
    QFile::remove(oldPath);
    QFile::rename(path, oldPath);
}

void AppLog::Write(const QString &category, const QString &message)
{
    if (g_appLogWriting) {
        return;
    }
    g_appLogWriting = true;

    const QString path = LogPath();
    RotateIfNeeded(path);

    QFile file(path);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out.setCodec("UTF-8");
        out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")
            << " [pid=" << QCoreApplication::applicationPid() << "] "
            << "[" << category << "] " << message << "\n";
    }

    g_appLogWriting = false;
}

void AppLog::Action(const QString &message)
{
    Write("ACTION", message);
}

void AppLog::Dialog(const QString &message)
{
    Write("DIALOG", message);
}

void AppLog::File(const QString &message)
{
    Write("FILE", message);
}

void AppLog::Error(const QString &message)
{
    Write("ERROR", message);
}

void AppLog::QtMessage(QtMsgType type, const char *msg)
{
    QString category;
    switch (type) {
    case QtDebugMsg:
        category = "QT_DEBUG";
        break;
    case QtWarningMsg:
        category = "QT_WARNING";
        break;
    case QtCriticalMsg:
        category = "QT_CRITICAL";
        break;
    case QtFatalMsg:
        category = "QT_FATAL";
        break;
    default:
        category = "QT";
        break;
    }
    Write(category, QString::fromLocal8Bit(msg ? msg : ""));
}
