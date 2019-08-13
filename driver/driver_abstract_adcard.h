#ifndef DRIVER_ABSTRACT_ADCARD_H
#define DRIVER_ABSTRACT_ADCARD_H

#include <QVector>

namespace driver {

template <typename T>
class AbstractADCard
{
public:
  explicit AbstractADCard(){}

  virtual ~AbstractADCard(){}
/***********************************************************************************
 * 函数: virtual bool init(int an, int dn, int sample=200000)=0
 * 描述: 初始化ad卡
 * 输入: int an: 模拟通道数
 * 输入: int dn: 数字通道数
 * 输入: int sample: 采样率
 * 返回: 成功返回true，否则false
 * *********************************************************************************/
  virtual bool init(int an, int dn, int sample=10000)=0;//打开设备

  //释放设备
  virtual bool release(){//释放设备
    return true;
  }

  virtual bool hasData()=0;//检测是否有数据


  virtual void next()=0;//采集下一次数


  //数据排列方式为先是模拟通道，然后是数字通道
  virtual const QVector<T> &getData()=0;


};
}
#endif // ADCARD_H
