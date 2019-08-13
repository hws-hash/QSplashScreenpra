#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTimer>
#include <QElapsedTimer>
#include<../driver/driver_t9342.h>
#include<scale/rt_thd.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
  void  InitCustomPlot();
   RtProcThread thread;

  void GetRealtimeData(RT_CTL* rtCtl, INT16 chData[16][10000]);
private:
    Ui::MainWindow *ui;
        int m_nTimerID;
        QTimer* m_ptimer;
        QElapsedTimer m_elapsedTimer;
        double m_lastPointKey;
         qint64 m_begin;
         INT16 chData[16][10000];

private slots:

          void  sigHandleTimeout();  //超时处理函数
};

#endif // MAINWINDOW_H
