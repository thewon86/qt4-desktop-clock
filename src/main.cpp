//#include "stdafx.h"
#include "tao.h"
#include <QtGui/QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//qDebug()<<a.desktop()->width();
	//qDebug()<<a.desktop()->height();
	tao *w = new tao;
	w->show();

	//TimerThread * tim_thread = new TimerThread(w);
	//a.connect(tim_thread, SIGNAL(finished()), &a, SLOT(quit()));
	//tim_thread->start();
	return a.exec();
}
