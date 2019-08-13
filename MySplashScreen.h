#ifndef _SPLASHSCREEN_H_
#define _SPLASHSCREEN_H_

#include <QSplashScreen>
#include <QMovie>

class QPixmap;
class QPainter;

class MySplashScreen : public QSplashScreen
{

public:
    MySplashScreen(const QPixmap & pixmap);
    MySplashScreen(const QString gifname);
    void setGif(QString filename);

protected:
     void drawContents(QPainter *painter);

private slots:
    void slot_update();

private:
    QMovie *m_move;
    int m_rotate;

};

#endif // _SPLASHSCREEN_H_
