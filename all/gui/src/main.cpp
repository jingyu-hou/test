 //      #include "ff1.h"
#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QTextCodec>
#include <iostream>
#include <QFont>
#include <QString>
#include <QPalette>
#include <QDesktopWidget>
#include <QTimer>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//ff1 w;
	//w.show();
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    if (!codec) codec = QTextCodec::codecForName("GB18030");
    if (!codec) codec = QTextCodec::codecForName("GBK");
    if (!codec) codec = QTextCodec::codecForName("GB2312");
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForTr(codec);
	QFont appFont;
#ifdef Q_OS_WIN32
	//appFont = QFont(a.font().family(), 10, QFont::Normal);
	appFont = QFont("宋体", 10, QFont::Normal);
#else
	//appFont = QFont("AR PL KaitiM GB", 11, QFont::Normal);
    appFont = QFont("WenQuanYi Zen Hei Mono", 11, QFont::Normal);
#endif
	a.setFont(appFont);


	MainWindow *w = new MainWindow;
	w->ForceShowMainWindow();
	QTimer::singleShot(500, w, SLOT(ForceShowMainWindow()));
	
	return a.exec();
}
