#include "driver_t9342thread.h"
#include <stdlib.h>
#include "fifo.h"
//#include <netinet/in.h>
#include <time.h>


#ifndef SERVER_IP
#define SERVER_IP         "192.168.0.200"
#endif
#ifndef SERVER_PORT
#define SERVER_PORT        9411
#endif

#define SAMPLE 80000



using namespace driver;

namespace  {
static struct {
  int an;//模拟通道数
  int dn;//数字通道数
  int sample;//采样率
 // int size_1s;//一秒钟的数据量,这是字节数
}s_param;
struct sockaddr_in s_address;//发送的ip地址
uv_udp_t s_udp;//发送的句柄
uv_loop_t s_loop;
struct fifo *s_fifo=0;




void alloc_cb(uv_handle_t* handle,
              size_t suggested_size,
              uv_buf_t* buf)
{
  static char slab[65536];

  buf->base = slab;
  buf->len = sizeof(slab);
}

void close_cb(uv_handle_t* handle){
  //uv_is_closing(handle);
  //free(handle);
}

void sv_send_cb(uv_udp_send_t* req, int status)
{
  //printf("sv_send_cb\n");
  //uv_close((uv_handle_t*) req->handle, close_cb);
  free(req);
  //  static bool first=true;

  //  if(first){
  //    first=false;
  //    startCollection(300,0,s_param.an,s_param.sample,0,1);
  //  }
}



void sv_recv_cb(uv_udp_t* handle,
                ssize_t nread,
                const uv_buf_t* rcvbuf,
                const struct sockaddr* addr,
                unsigned flags){
  if (nread <= 0){
    return;
  }
  //printf("data\n");
  static int next=-1;
  static int current;
  if(rcvbuf->base[0] == 0x55 && nread>6){
    /************************检测帧编号**********************************************/
    current=rcvbuf->base[3]&0xff;
    current<<=8;
    current +=rcvbuf->base[2]&0xff;
    if(next != -1 && next != current){
//      SYSTEMTIME currentTime;
//      GetSystemTime(&currentTime);
//      printf("time: %u/%.2u/%.2u %.2u:%.2u:%.2u:%.3u : current:%d next:%d\n",
//             currentTime.wYear,currentTime.wMonth,currentTime.wDay,
//             currentTime.wHour,currentTime.wMinute,currentTime.wSecond,
//             currentTime.wMilliseconds,current,next);
            time_t timer;
            struct tm* t_tm;
            time(&timer);
            t_tm = localtime(&timer);
            //printf("today is %4d%02d%02d%02d%02d%02d/n", t_tm->tm_year+1900,
           // t_tm->tm_mon+1, t_tm->tm_mday, t_tm->tm_hour, t_tm->tm_min, t_tm->tm_sec);
    }

    next=current+1;
    next= next %0xffff;
    /************************end 检测帧编号**********************************************/
    uint8_t sum1;
    const int size=(nread-6)/sizeof(uint16_t);//计算所有通道采样点之和
    uint16_t *p16=(uint16_t *)rcvbuf->base;
    p16 += 3;
    uint16_t sum0,value;
    sum0=0;
    for(int i=0; i<size; ++i){
      value=p16[i];
      sum0 += value;
    }
    sum1=rcvbuf->base[1];
    if((uint8_t)sum0 != sum1){//计算和校验失败
      printf("sum error\n");
      return;
    }

    fifo_put(s_fifo, (const unsigned char*)(rcvbuf->base+6), size*sizeof(uint16_t));

  }else{
    //printf("data[0] != 0x55");
  }


  //  static int cnt=0;
  //  cnt += (nread-6)/(s_param.an+1)/2;
  //  if(cnt >=s_param.sample){
  //    cnt=0;
  //        SYSTEMTIME currentTime;
  //        GetSystemTime(&currentTime);
  //            printf("time: %u/%.2u/%.2u %.2u:%.2u:%.2u:%.3u  nread:%d\n",
  //                   currentTime.wYear,currentTime.wMonth,currentTime.wDay,
  //                   currentTime.wHour,currentTime.wMinute,currentTime.wSecond,
  //                   currentTime.wMilliseconds,nread);
  //  }
}





bool open(int an, int dn, int sample, int size){//打开设备
  if(an<1 || an>64 || dn%8 !=0 || dn>16 || dn<=0){
    printf("channel is error.\nan:%d dn:%d",an,dn);
    return false;
  }


  int r,i;
  struct sockaddr_in addr;//本地绑定的ip地址
  uv_loop_init(&s_loop);


  for(i=8899; i<60000; ++i){
    r = uv_ip4_addr("0.0.0.0", i, &addr);
    r = uv_udp_init(&s_loop, &s_udp);
    r = uv_udp_bind(&s_udp, (const struct sockaddr*) &addr, 0);
    if(r == 0){//绑定成功
      printf("port:%d sample:%d\n",i,sample);
      break;
    }
  }

  //要采集卡的ip地址
  r = uv_ip4_addr(SERVER_IP, SERVER_PORT, &s_address);

  s_param.an=an;
  s_param.dn=dn;
  s_param.sample=sample;
  s_fifo=fifo_alloc(size*(s_param.an+1)*sizeof(uint16_t));
  if( !s_fifo ){
    return false;
  }

  return true;
}


int send(char *data, int len){
  uv_udp_send_t *req = (uv_udp_send_t *)malloc(sizeof(uv_udp_send_t));
  uv_buf_t msg = uv_buf_init(data,len);
  int r=uv_udp_send(req, &s_udp, &msg, 1, (const struct sockaddr *)&s_address, sv_send_cb);
  if(r  == 0){
    return len;
  }
  return r;
}


void startCollection(uint32_t sample_number, uint16_t begin_channel, uint16_t channel_number,
                     uint32_t sample_rate, uint32_t freq_filter,uint16_t ampgain){
  char byte[24];
  uint16_t cmd=1;//命令编号
  uint16_t cmd_len=18;//命令长度
  byte[0]=0xaa;//命令开始标识
  byte[1]=cmd>>8;
  byte[2]=cmd&0xff;
  byte[3]=cmd_len>>8;
  byte[4]=cmd_len&0xff;

  byte[5]=sample_number>>24;
  byte[6]=sample_number>>16;
  byte[7]=sample_number>>8;
  byte[8]=sample_number&0xff;

  byte[9]=begin_channel>>8;
  byte[10]=begin_channel&0xff;

  byte[11]=channel_number>>8;
  byte[12]=channel_number&0xff;

  byte[13]=sample_rate>>24;
  byte[14]=sample_rate>>16;
  byte[15]=sample_rate>>8;
  byte[16]=sample_rate&0xff;

  byte[17]=freq_filter>>24;
  byte[18]=freq_filter>>16;
  byte[19]=freq_filter>>8;
  byte[20]=freq_filter&0xff;

  byte[21]=ampgain>>8;
  byte[22]=ampgain&0xff;

  uint8_t sum,value;
  sum=0;
  for(int i=0; i<18; ++i){
    value=byte[i+5];
    sum += value;
  }
  byte[23]=sum;
  send(byte,24);
  QByteArray byteArray;
        for(int i=0;i<24;i++)
        {
          byteArray.append(byte[i]);
        }
      QUdpSocket udpSocket;
      QHostAddress addr;
      addr.setAddress(SERVER_IP);
     udpSocket.writeDatagram(byteArray, addr,SERVER_PORT);//2018-9-22
}


void stopCollection(){
  char byte[6];
  uint16_t cmd=0xaa;//命令编号
  uint16_t cmd_len=0;//命令长度
  byte[0]=0xaa;//命令开始标识
  byte[1]=cmd>>8;
  byte[2]=cmd&0xff;
  byte[3]=cmd_len>>8;
  byte[4]=cmd_len&0xff;
  byte[5]=0;//校验和
  send(byte,6);
}

void keep(){
  char byte[6];
  uint16_t cmd=0;//命令编号
  uint16_t cmd_len=0;//命令长度
  byte[0]=0xaa;//命令开始标识
  byte[1]=cmd>>8;
  byte[2]=cmd&0xff;
  byte[3]=cmd_len>>8;
  byte[4]=cmd_len&0xff;
  byte[5]=0;//校验和
  send(byte,6);
}

void timer_cb(uv_timer_t *handle) {
  //printf("timeout\n");
  keep();
}

}



T9342thread::T9342thread(QObject *parent):QThread(parent)
{
}


bool T9342thread::init(int an, int dn, int sample, int size){//打开设备
  if(an<1 || an>64 || (dn != 8 && dn != 16)){
    log(QString().sprintf("channel is error.\nan:%d dn:%d",an,dn));
    return false;
  }

  if(!open(an,dn,sample,size)){
    return false;
  }
  m_data.resize(sample*(an+1));
  start(QThread::TimeCriticalPriority);
  return true;
}


void T9342thread::release(){
  uv_udp_recv_stop(&s_udp);
  quit();
  wait();
  fifo_free(s_fifo);
  s_fifo=0;
}


void T9342thread::run(){
  setbuf(stdout, NULL);
  if(uv_udp_recv_start(&s_udp, alloc_cb, sv_recv_cb) != 0){
    return ;
  }

  stopCollection();
  startCollection(s_param.sample,0,s_param.an+1,s_param.sample,0,1);
  uv_timer_t timer;

  uv_timer_init(&s_loop, &timer);

  uv_timer_start(&timer, timer_cb, 1000, 1000);
  uv_run(&s_loop, UV_RUN_DEFAULT);
}


int T9342thread::getSampleSize(){
  if(s_fifo == 0){
    return -1;
  }
  unsigned len=fifo_len(s_fifo);
  return len/(s_param.an+1)/2;
}


const QVector<uint16_t> &T9342thread::getData(){
  Q_ASSERT(s_fifo != 0);
  fifo_get(s_fifo,(unsigned char*)m_data.data(),m_data.size()*sizeof(uint16_t));
  //qDebug()<<m_data[1];
  return m_data;
}

