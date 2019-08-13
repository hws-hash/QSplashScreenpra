
#include "rt_thd.h"
#include "fltrcd.h"
//#include "MergeMgr.h"
#include <math.h>
#include <stdlib.h>
//#include "gpio.h"
//#include "rt_data_mgr.h"
#include"QDebug"
#include "assert.h"
#include "driver/driver_adcard16.h"
#include"rt_ctl.h"




#if (0)
	#include "rt_data_mgr.h"
	#include "ch_adjust_mgr.h"
#endif

RtProcThread::RtProcThread()//QObject *parent
{



}

RtProcThread::~RtProcThread()
{
		//pci1706.PCI_Release();	//释放采集卡
    //dog.closeWtg();
}

void RtProcThread::run()
{


      static unsigned short *pDstData = NULL;
      static int svtime = 0;

      g_rtCtl->AnChCnt=16;
      g_rtCtl->BinChCnt=8;
      driver::ADCard16 *ad=driver::ADCard16::instance();
      if(!ad->init(g_rtCtl->AnChCnt,g_rtCtl->BinChCnt,10000)){
        printf( "adcard Init failed!\n" );
      }


   // const int bin_group=(g_rtCtl->BinChCnt+15)/16;//数字量的组数
    #if  defined(YHF_VIRTUAL_DATA) || defined(DRIVER_ADCARD)
        static struct timeval smptv;
        gettimeofday(&smptv, NULL);
    #endif




      msleep( 2000 );
        while( 1 )
        {
            msleep( 100 );
            if ( 0 )	//ctm 标志退出
            {
          ad->release();

                break;
            }

               //ctm 等待数据到达
    #ifdef YHF_VIRTUAL_DATA
    #ifndef DRIVER_ADCARD
            msleep( 700 );
            pci1706.DataHandler(NULL, NULL);
        #else
        //pci1706.bufferReady=pci1706.rh_adcard.bufferReady;
        if ( pci1706.bufferReady != true){
          svtime++;
          if(svtime > 15){
            printf("pci1706_ReadData failed! svtime = %d\n", svtime);
          }
          continue;
        }else{
          svtime = 0;
          pci1706.bufferReady = false;
          //pci1706.rh_adcard.bufferReady=false;
        }
    #endif
    #else
        //if ( pci1706->bufferReady != true)
        if(!ad->hasData()){
                svtime++;
          if(svtime > 18)
                {
                    printf("pci1706_ReadData failed! svtime = %d\n", svtime);
                    g_rtCtl->swFlg=0;
                }
                continue;
            }
            else
            {
                svtime = 0;

            }
    #endif

    #ifdef YHF_VIRTUAL_DATA
        #ifndef DRIVER_ADCARD
            memcpy(g_rtCtl->CurSmpPntAddr, &smptv, sizeof(struct timeval));
        smptv.tv_sec++;
    #else
        gettimeofday((timeval *)(g_rtCtl->CurSmpPntAddr), NULL);
    #endif
    #else
            gettimeofday((timeval *)(g_rtCtl->CurSmpPntAddr), NULL);
    #endif

        const QVector<quint16> &pSrcData=ad->getData();
            //g_rtCtl->CurSmpPntAddr += sizeof(YHFtimeval);
            pDstData = (unsigned short *)(g_rtCtl->CurSmpPntAddr + sizeof(YHFtimeval));	//ctm
        for (int count=0,i=0; i<g_rtCtl->SmpCntPerSec; i++)
            {
           //memcpy(g_rtCtl->CurSmpPntAddr, pSrcData, 8);	//ctm g_rtCtl->AnSmpLenPerPnt
          for(int j=0; j<g_rtCtl->AnChCnt; j++)
                {

            *(pDstData++) = pSrcData[count++];

                }
          for(int j=0; j<1; ++j){
            *(pDstData++) = pSrcData[count++];
          }
          pDstData +=  sizeof(YHFtimeval)/2;
            }
        ad->next();
        g_rtCtl->CurSmpPntAddr += g_rtCtl->SmpDataLenPerPnt * g_rtCtl->SmpCntPerSec;
        g_rtCtl->CurSmpPntPos += g_rtCtl->SmpCntPerSec;
            if (g_rtCtl->CurSmpPntPos >= g_rtCtl->SmpPntCnt)
            {
                g_rtCtl->CurSmpPntPos = 0;
                g_rtCtl->CurSmpPntAddr = g_rtCtl->SmpBufAddr;
            }
}
}



