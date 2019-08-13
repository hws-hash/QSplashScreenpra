#include "driver_adcard16.h"
#include <QFile>
#include <QSettings>
#include "driver_virtualad16.h"
#include "driver_t9342.h"


#define DRIVER_VIRTUAL     0//ÐéÄâÇý¶¯±àºÅ
#define DRIVER_RHB8362     1//RHB8362ÐÂÇý¶¯±àºÅ
#define DRIVER_RHB8362_7   2//RHB8362ÐÂÇý¶¯±àºÅ

using namespace driver;

ADCard16 *ADCard16::instance(){
  static ADCard16 *ad=0;
  if(ad == 0){
//    QString card=config::Setting::instance()->get().adcard;
//    if(card == "RHB8632"){
//      ad=dynamic_cast<ADCard16 *>(RHB8362::instance());
//    }else if(card == "RHB8362_7"){
//      ad=dynamic_cast<ADCard16 *>(RHB8362_7::instance());
//    }else if(card == "RHB8362_7x"){
//      ad=dynamic_cast<ADCard16 *>(RHB8362_7x::instance());
//      driver::Channel *channel=driver::Channel::instance();
//      if(channel != 0){
//        channel->setCustom(true);
//      }
//    }else{
//      ad=dynamic_cast<ADCard16 *>(new VirtualAD16());
//    }
    ad=T9342::instance();
    // ad=dynamic_cast<ADCard16 *>(new VirtualAD16());
  }
  return ad;
}
