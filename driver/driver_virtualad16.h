#ifndef DRIVER_VIRTUALAD16_H
#define DRIVER_VIRTUALAD16_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QObject>
#include "driver_adcard16.h"
namespace driver {
class VirtualAD16 : public QObject,public ADCard16
{
  Q_OBJECT
public:
  VirtualAD16(QObject *parent = 0);

  /***********************************************************************************
   * 函数: virtual bool init(int an, int dn, int sample=200000)=0
   * 描述: 初始化ad卡
   * 输入: int an: 模拟通道数
   * 输入: int dn: 数字通道数
   * 输入: int sample: 采样率
   * 返回: 成功返回true，否则false
   * *********************************************************************************/
  virtual bool init(int an, int dn, int sample=200000);//打开设备

  virtual bool hasData(){//检测是否有数据
    return m_flag;
  }

  virtual void next(){//采集下一次数
    m_flag=false;
  }

  virtual const QVector<quint16> &getData(){
    return m_data;
  }

  void toggle();//翻转开关位

private slots:

  void timerSlot(){
    if(m_flag) {
      return;
    }
    m_flag=true;
  }

private:

  bool m_flag;//是否有数据的标志
  QTimer m_timer;
  QVector<quint16> m_data;
  int m_an;//模拟通道数
  int m_count;//总的组数
  int m_sample;//采样点数

  void log(const QString &str){
    qDebug()<<"VirtualAD16: "<<str;
  }
};
}
#endif // VIRTUALAD16_H
