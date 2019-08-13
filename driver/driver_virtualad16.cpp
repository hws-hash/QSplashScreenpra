#include "driver_virtualad16.h"
#include <math.h>

using namespace driver;

VirtualAD16::VirtualAD16(QObject *parent):QObject(parent),m_flag(false)
{

}

bool VirtualAD16::init(int an, int dn, int sample){
  if(an %8 != 0 || an<1 || an>64 || dn %8 != 0 || dn<0 || dn>64 ){
    log(QString().sprintf("channel is error.\nan:%d dn:%d",an,dn));
    return false;
  }
  double PI = 3.1415926535897932384626433832795;
  int amp = sample;
  int group=an/8;//以8通道为一组的组数
  int count=(dn+15)/16+an;
  m_data.resize(count*sample);
  for(int i=0; i<sample; i++) {
    for(int j=0; j<group; j++){

      m_data[i*count+j*8]   = (20000 * cos(2*PI*i/(amp/50) + 2*PI/3) + 32768);
      m_data[i*count+j*8+1] = (20000 * cos(2*PI*i/(amp/50) + 2*PI/3+0.1) + 32768);
      m_data[i*count+j*8+2] = (20000 * cos(2*PI*i/(amp/50) + 2*PI/3+0.2) + 32768);
      m_data[i*count+j*8+3] = (20000 * cos(2*PI*i/(amp/50) + 2*PI/3+0.3) + 32768);

      m_data[i*count+j*8+4] = (20000 * cos(2*PI*i/(amp/50) + 2*PI/3+0.11) + 32768);
      m_data[i*count+j*8+5] = (20000 * cos(2*PI*i/(amp/50) + 2*PI/3+0.12) + 32768);
      m_data[i*count+j*8+6] = (20000 * cos(2*PI*i/(amp/50) + 2*PI/3+0.13) + 32768);
      m_data[i*count+j*8+7] = (20000 * cos(2*PI*i/(amp/50) + 2*PI/3+0.14) + 32768);

      //m_data[i*count+j*8]   = (20000 * cos(2*PI*i/(amp/50) + 2*PI/3) + 32768);
      //m_data[i*count+j*8+1] = (20000 / 10 * cos(2*PI*i/(amp/50) - 2*PI/3) + 32768);
      //m_data[i*count+j*8+2] = (20000 * cos(2*PI*i/(amp/50) + 2*PI/3) + 32768);
      //m_data[i*count+j*8+3] = (20000 / 10 * cos(2*PI*i/(amp/50) - 2*PI/3) + 32768);

      //m_data[i*count+j*8+4] = 0;
      //m_data[i*count+j*8+5] = (20000 * sin(2*PI*i/(amp/50) + 2*PI/3) + 32768);
      //m_data[i*count+j*8+6] = (20000 / 10 * cos(2*PI*i/(amp/50) - 2*PI/3) + 32768);
      //m_data[i*count+j*8+7] = (20000 * sin(2*PI*i/(amp/50) + 2*PI/3) + 32768);
    }
    for(int j=an; j<count; j++){
      m_data[i*count+j]   = 0x0000;
    }
  }
  m_an=an;
  m_count=count;
  m_sample=sample;
  connect(&m_timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
  //  timer.setInterval(500);
  m_timer.start(1000);
  return true;
}

void VirtualAD16::toggle(){
  for(int i=0; i<m_sample; i++) {
    for(int j=m_an; j<m_count; j++){
      m_data[i*m_count+j]   = ~m_data[i*m_count+j];
    }
  }
}
