#ifndef DRIVER_T9342_H
#define DRIVER_T9342_H

#include <QVector>
#include <QProcess>
#include <QtNetwork>
#include <QDebug>
#include <QTimer>
#include "driver_adcard16.h"
#include "driver_t9342thread.h"

namespace driver {

class T9342 :public QObject, public ADCard16
{
  Q_OBJECT
public:
  static T9342 *instance(){
    static T9342 *p=0;
    if(p == 0){
      p = new T9342();
    }
    return p;
  }

  virtual ~T9342(){
    release();
    if(m_process[0]){
      delete m_process[0];
    }
    if(m_process[1]){
      delete m_process[1];
    }
  }

  virtual bool init(int an, int dn, int sample=200000);//打开设备

  //释放设备
  virtual bool release();

  virtual bool hasData();//检测是否有数据

  virtual void next(){//采集下一次数
  }

  virtual const QVector<quint16> &getData();//获取数据

private slots:
//  void readSlot();
//  void timeoutSlot();

protected:
  T9342();

private:
  QVector<quint16> m_data;
  QProcess *m_process[2];
  T9342thread m_thread;
  int m_sample;//采样率
  int m_an;//模拟通道数


private:

  void log(const QString &str){
    qDebug()<<"T9342: "<<str;
  }

//  /*******************************************************************************************
//   * 函数: qint64 send(const QByteArray &byteArray)
//   * 描述: 发送数据或命令到采集卡
//   * 输入: const QByteArray &byteArray: 要发送的数据或命令
//   * 返回: 发送的字节数
//   * *****************************************************************************************/
//  qint64 send(const QByteArray &byteArray);

//  /*******************************************************************************************
//   * 函数:  void start(quint32 sample_number, quint16 begin_channel, quint16 channel_number,
//             quint32 sample_rate, quint32 freq_filter,quint16 ampgain)
//   * 描述: 开始采集数据
//   * 输入: quint32 sample_number: 每个通道的采样点数
//   * 输入: quint16 begin_channel: 开始通道
//   * 输入: quint16 channel_number: 要采样的通道数
//   * 输入: quint32 sample_rate: 采样率
//   * 输入: quint32 freq_filter:????
//   * 输入: quint16 ampgain:????
//   * *****************************************************************************************/
//  void start(quint32 sample_number, quint16 begin_channel, quint16 channel_number,
//             quint32 sample_rate, quint32 freq_filter,quint16 ampgain);

//  void stop();//停止采集数据

//  void keep();//保持连接
};

}
#endif // DRIVER_T9342_H
