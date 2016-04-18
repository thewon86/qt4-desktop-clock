#ifndef TAO_H
#define TAO_H

#include <QWidget>
#include <QSystemTrayIcon>
#include "worldtimeclock.h"
//#include "ui_tao.h"
QT_BEGIN_NAMESPACE
class QPoint;
class QPixmap;
class QEvent;
class QTimer;
QT_END_NAMESPACE

class tao : public QWidget
{
	Q_OBJECT

public:
	tao(QWidget *parent = 0, Qt::WFlags flags = 0);
	~tao();

Q_SIGNALS:
	void PosChanged(QPoint &new_pos);

private Q_SLOTS:
	void ChangeSkin();
	void QuitPrag();
	void TrayIconActived(QSystemTrayIcon::ActivationReason reason);
	void checkpos();
    void updateTrayIconMenu();

protected:
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void paintEvent(QPaintEvent *);
	bool event(QEvent *);

private:
	void InitData();
	void InitVisualComponents();
	void SetupContextMenu();
    void ScanImg();

private:
	QPoint* Drag_Pos;
	QPoint* old_pos;
	QPixmap* BG_Pixmap;
	QPixmap* BG_Pixmap_bk;
	QPixmap* BG_Pixmap_paint;
	QSystemTrayIcon *trayIcon;
	WorldTimeClock *myClock;
	QTimer *mtimer;
	bool show_time;
	bool IsShow;
    QStringList *imgList;
    QPixmap *m_blackImg;
};

#endif // TAO_H
