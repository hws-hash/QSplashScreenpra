#ifndef DRIVER_T9342THREAD_H
#define DRIVER_T9342THREAD_H

#include <QThread>
#include <QtNetwork>
#include "./libuv/uv.h"
#include <stdint.h>


namespace driver {


class T9342thread: public QThread
{
public:
  explicit T9342thread(QObject *parent = 0);

  /***********************************************************************************
   * 函数: virtual bool init(int an, int dn, int sample=200000)=0
   * 描述: 初始化ad卡
   * 输入: int an: 模拟通道数
   * 输入: int dn: 数字通道数
   * 输入: int sample: 采样率
   * 输入: int size: 最大缓存采样点数
   * 返回: 成功返回true，否则false
   * *********************************************************************************/
  bool init(int an, int dn, int sample, int size);//打开设备
  void release();

  //获取采样点数，返回负数表示出错
  int getSampleSize();

  //获取1秒时间的数据
  const QVector<uint16_t> &getData();

  static void log(const QString &str){
    static int cnt=0;
    qDebug()<<"T9342thread "<<cnt++<<": "<<str;
  }



protected:
  void run();

private:
  QVector<uint16_t> m_data;

};

}

#endif // DRIVER_T9342THREAD_H
