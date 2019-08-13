#include "mainwindow.h"
#include <QApplication>
#include"../scale/sg_mgr.h"
#define  	SG_FNAME			"/home/hd/conf/YHFDevice.cf"
#define	SG_FNAME_BACKUP	"/home/hd/conf/YHFDevBack.cf"
#include <QApplication>
#include <QSplashScreen>
#include<QPainter>
#include<MySplashScreen.h>


SGManager g_sgMgr;
int main(int argc, char *argv[])
{

    g_sgMgr.createDfrSGFromFile(SG_FNAME);

    if ( g_sgMgr.m_psg == NULL ){
      printf( "load SGFile [%s] failed!\n", SG_FNAME );
      g_sgMgr.createDfrSGFromFile(SG_FNAME_BACKUP);
      if ( g_sgMgr.m_psg == NULL )                        {
        printf( "load SGFile [%s] failed!\n", SG_FNAME_BACKUP );
        //g_sgMgr.m_psg = g_sgMgr.createDfrSGData(12, 0, 1, 1, 0, 0);
        g_sgMgr.m_psg = g_sgMgr.createDefaultDfrSG();
        if (g_sgMgr.m_psg == NULL){
          return -1;
        }
        g_sgMgr.createDfrSGFile( SG_FNAME, g_sgMgr.m_psg );
      }
    }
    int ret;
    ret = fltrcd_init( g_sgMgr.m_psg );
    if ( ret != 0 )
    {
      printf( "fltrcd_init() failed!\n" );
      return ret;
    }

    QApplication a(argc, argv);

    QPixmap pix(":/timg.jpg");
   // QSplashScreen  splash(pix);
    MySplashScreen splash(pix);
    splash.showMessage("Wait...");

//      Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
//      splash.showMessage(QObject::tr("Setting up the main window..."),topRight, Qt::white);//显示信息
    splash.show();

    QDateTime n=QDateTime::currentDateTime();

    QDateTime now;

    do{

        now=QDateTime::currentDateTime();

    } while (n.secsTo(now)<=3);//6为需要延时的秒数
   // //使程序在显示启动画面的同时仍能响应鼠标其他事件
  //  a.processEvents();


    MainWindow w;
    w.show();

    splash.finish(&w);//程序启动画面结束

    return a.exec();
}
