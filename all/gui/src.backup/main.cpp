 //      #include "ff1.h"
#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>
#include <iostream>
#include <QFont>
#include <QString>
#include <QPalette>
#include <QDesktopWidget>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//ff1 w;
	//w.show();
#if _MSC_VER

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));//("system"));

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));//("system"));

    QTextCodec::setCodecForTr(QTextCodec::codecForName("system"));//("system"));
#else
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("system"));//("system"));

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("system"));//("system"));

    QTextCodec::setCodecForTr(QTextCodec::codecForName("system"));//("system"));
#endif
	QFont appFont;
#ifdef Q_OS_WIN32
	//appFont = QFont(a.font().family(), 10, QFont::Normal);
	appFont = QFont("����", 10, QFont::Normal);
#else
	//appFont = QFont("AR PL KaitiM GB", 11, QFont::Normal);
    appFont = QFont("Microsoft YaHei", 11, QFont::Normal);
#endif
	a.setFont(appFont);


	MainWindow *w = new MainWindow;
	w->setWindowTitle("WeICME");
	w->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
	w->setAttribute(Qt::WA_NativeWindow, true);
	w->showMaximized();
	w->raise();
	w->activateWindow();
	std::cout << "WeICME main visible=" << w->isVisible()
	          << " geom=" << w->geometry().x() << "," << w->geometry().y()
	          << "," << w->geometry().width() << "x" << w->geometry().height()
	          << " winId=" << (unsigned long)w->winId() << std::endl;
	
	return a.exec();
}
