#include <QPixmap>
#include <QPainter>
#include<MySplashScreen.h>
#include <QLabel>
#include <QTimer>

MySplashScreen::MySplashScreen(const QPixmap & pixmap) : QSplashScreen(pixmap)
{

}

MySplashScreen::MySplashScreen( const QString gifname )
{
    setGif(gifname);
}


void MySplashScreen::slot_update()
{
    setPixmap(m_move->currentPixmap());
    repaint();
}

void MySplashScreen::drawContents(QPainter *painter)
{
    painter->setFont(QFont("SimHei", 40));
    painter->setPen(QColor(213, 218, 220));
    painter->drawText(QPointF(20, 100), "test system");

    painter->setFont(QFont("SimHei", 18));
    painter->setPen(QColor(213, 218, 220));
    painter->drawText(QPointF(30, 140), "Version: 1.0.0");

    painter->setFont(QFont("Helvetica", 16));
    painter->setPen(QColor(Qt::white));
    QRect r = rect();
    r.setRect(r.x(), r.y(), r.width(), r.height() -12);
    painter->drawText(r, Qt::AlignBottom | Qt::AlignCenter, "test");

    painter->setFont(QFont("Verdana", 11));
    QSplashScreen::drawContents(painter);
}

void MySplashScreen::setGif( QString filename )
{
    m_move = new QMovie(filename);
    m_move->start();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slot_update()));
    timer->start(100);
}
