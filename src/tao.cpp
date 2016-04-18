//#include "stdafx.h"
#include <QtGui>
#include "tao.h"
#include <QEvent>
#include <QTime>
#include <qmath.h>
#include <QSystemTrayIcon>
#include <Windows.h>
#include <QDir>
#include <QFileInfo>
#include <QLabel>

tao::tao(QWidget *parent, Qt::WFlags flags):
    QWidget(parent, flags),
    Drag_Pos(new QPoint),
    BG_Pixmap(new QPixmap),
    BG_Pixmap_bk(new QPixmap),
    imgList(new QStringList),
    m_blackImg(new QPixmap),
    trayIcon(new QSystemTrayIcon(this))
{
	//ui.setupUi(this);
	old_pos = new QPoint;
	*old_pos = this->frameGeometry().topLeft();
	myClock = new WorldTimeClock(this);
	myClock->hide();
	InitData();
	InitVisualComponents();
    ScanImg();
    SetupContextMenu();
    trayIcon->setIcon(QIcon(*BG_Pixmap));
    trayIcon->setToolTip(tr("My Desktop Clock"));
    trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(TrayIconActived(QSystemTrayIcon::ActivationReason)));

    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
    setAutoFillBackground(0);
    move(qApp->desktop()->width() - 256, 0);

    setAttribute(Qt::WA_DeleteOnClose);
	setAttribute(Qt::WA_TranslucentBackground);

	mtimer = new QTimer;
    mtimer->start(500);
    connect(mtimer, SIGNAL(timeout()), this, SLOT(checkpos()));
	show_time = false;
	IsShow = true;

}

tao::~tao()
{
	//delete trayIcon;
	//delete Drag_Pos;
	//delete BG_Pixmap;
}

void tao::QuitPrag()
{
	trayIcon->setVisible(false);
	delete trayIcon;
	delete Drag_Pos;
	delete BG_Pixmap;
	delete BG_Pixmap_bk;
	qApp->quit();
}

void tao::ChangeSkin()
{
	QAction* src = qobject_cast<QAction*>(sender());
//    qDebug()<<src->data().toString();
    delete BG_Pixmap;
    BG_Pixmap = new QPixmap;
    BG_Pixmap->load(src->data().toString());

    int w1 = BG_Pixmap->width();
    int h1 = BG_Pixmap->height();
    int w2 = this->width();
    int h2 = this->height();

    QPixmap img = *m_blackImg;
    QPainter tpt_t(&img);
    if(w1>w2||h2>w2){
        QPixmap img_t = BG_Pixmap->scaled(256, 256);
        tpt_t.drawPixmap(0,0,w2,h2,img_t);
    }
    else if(w1<w2||h2<w2){
        tpt_t.drawPixmap((w2-w1)/2, (h2-h1)/2,w1,h1,*BG_Pixmap);
    }
    else {tpt_t.drawPixmap(0,0,w2,h2,*BG_Pixmap);}

    *BG_Pixmap = img;
    BG_Pixmap_paint = BG_Pixmap;
	clearMask();
    setMask(BG_Pixmap->mask());
    setWindowIcon(QIcon(*BG_Pixmap));
    trayIcon->setIcon(QIcon(*BG_Pixmap));
	update();
}

void tao::InitData()
{
//	Drag_Pos = new QPoint;
    BG_Pixmap->load(tr(":/yoda"));
    BG_Pixmap_bk->load(tr(":/black"));
    m_blackImg->load(tr(":/black"));
	BG_Pixmap_paint = BG_Pixmap;
}

void tao::InitVisualComponents()
{
	this->setObjectName(tr("mclock"));
	this->setWindowTitle(tr("My Desktop Clock"));
	this->resize(BG_Pixmap->width(), BG_Pixmap->height());
	this->setMask(BG_Pixmap->mask());
}

void tao::ScanImg()
{
//    QStringList *imgList;
    QDir skinsDir_t(QString("./"));
    if(!skinsDir_t.exists(QString("./skins"))){
        return;
    }
    skinsDir_t.setPath(QString("./skins"));

    *imgList = skinsDir_t.entryList(QDir::Files, QDir::Name);

//    foreach (QString ifile, *imgList) {
//        qDebug() << ifile;
//    }
}

void tao::SetupContextMenu()
{
    QMenu* trayIconMenu = new QMenu(this);
//    if(imgList->isEmpty()){
//        return;
//    }
    foreach (QString ifile, *imgList) {
        QString filePath(QString("./skins/") + ifile);
        QFileInfo fileinfo_t(filePath);
//        QString baseName = fileinfo_t.baseName();
//        qDebug() << baseName;
        QAction *trayIconAct = new QAction(QIcon(filePath), fileinfo_t.baseName(), this);
        trayIconAct->setData(filePath);
        connect(trayIconAct, SIGNAL(triggered()), this, SLOT(ChangeSkin()));
        trayIconMenu->addAction(trayIconAct);
    }

	QAction* quit_act = new QAction(tr("&Quit"), this);
    connect(quit_act, SIGNAL(triggered()), this, SLOT(QuitPrag()));

    QAction* update_act = new QAction(tr("&Update"), this);
    connect(update_act, SIGNAL(triggered()), this, SLOT(updateTrayIconMenu()));

    trayIconMenu->addSeparator();
    trayIconMenu->addAction(update_act);
	trayIconMenu->addAction(quit_act);

    trayIcon->setContextMenu(trayIconMenu);
}

void tao::mousePressEvent(QMouseEvent *evn)
{
	if (evn->button() == Qt::LeftButton)
	{
		evn->accept();
		*Drag_Pos = evn->globalPos() - frameGeometry().topLeft();
	}
}

void tao::mouseMoveEvent(QMouseEvent *evn)
{
	//qDebug()<<evn->pos();
	if (evn->buttons() & Qt::LeftButton)
	{
		evn->accept();
		move(evn->globalPos() - *Drag_Pos);
	}
}

void tao::mouseReleaseEvent(QMouseEvent *evn)
{

}

void tao::paintEvent(QPaintEvent *)
{
    QPainter tpt(this);
//    QLabel *pix = new QLabel;
//    pix->setPixmap(*BG_Pixmap_paint);
//    pix->show();

    tpt.fillRect(0, 0, BG_Pixmap_bk->width(), BG_Pixmap_bk->height(), *BG_Pixmap_bk);
    tpt.fillRect(0, 0, BG_Pixmap_paint->width(), BG_Pixmap_paint->height(), *BG_Pixmap_paint);
}

bool tao::event(QEvent *evn)
{
	if (evn->type() == QEvent::Enter)
    {
        evn->accept();
		if (!show_time)
        {
            BG_Pixmap_paint = BG_Pixmap_bk;
			clearMask();
			setMask(BG_Pixmap_bk->mask());
			setWindowIcon(QIcon(*BG_Pixmap_bk));
			update();
			myClock->show();
            show_time = true;
		}
		return true;
	} 
	else if (evn->type() == QEvent::Leave)
    {
        evn->accept();
		return true;
	}
    else return QWidget::event(evn);

}

void tao::checkpos()
{
	QPoint cursor_pos = QCursor::pos();

	if (show_time)
	{
		QPoint cur_pos = this->frameGeometry().topLeft();
		QPoint center_pos = cur_pos + QPoint(this->width() / 2, this->height() / 2);
		
		if ((qPow((cursor_pos.x() - center_pos.x()), 2) + qPow((cursor_pos.y() - center_pos.y()), 2)) > qPow(128, 2))
		{
			//qDebug()<<cursor_pos;
            myClock->hide();
            BG_Pixmap_paint = BG_Pixmap;
			clearMask();
			setMask(BG_Pixmap->mask());
			setWindowIcon(QIcon(*BG_Pixmap));
			update();
            show_time = false;
		}
    }
}

void tao::updateTrayIconMenu()
{
    ScanImg();
    SetupContextMenu();
}

void tao::TrayIconActived(QSystemTrayIcon::ActivationReason reason)
{
//    qDebug()<<reason;
	switch(reason)
	{
	case QSystemTrayIcon::Unknown:
		break;
    case QSystemTrayIcon::Context:
		break;
    case QSystemTrayIcon::DoubleClick:
        IsShow = !IsShow;
		if (IsShow)
		{
			this->show();
		}
		else
			this->hide();
		break;
	case QSystemTrayIcon::Trigger:
		break;
	case QSystemTrayIcon::MiddleClick:
		break;
	default:
		break;
	}
}
