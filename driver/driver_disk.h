#ifndef DRIVER_DISK_H
#define DRIVER_DISK_H
#include <windows.h>
#include <QString>

namespace driver {

class Disk
{
public:

  /***********************************************************************************
   * 函数: quint64 freeSpace(const QString &driver)
   * 描述: 获取windows分区的剩余大小
   * 输入: const QString &driver: 磁盘符,例如: "C:/"
   * 返回: 磁盘剩余的字节数
   * *********************************************************************************/
  static quint64 freeSpace(const QString &driver){
    LPCWSTR lpcwstrDriver=(LPCWSTR)driver.utf16();
    ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
    if( !GetDiskFreeSpaceEx( lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes) ){
      //qDebug() << "ERROR: Call to GetDiskFreeSpaceEx() failed.";
      return 0;
    }
    return (quint64) liTotalFreeBytes.QuadPart;
  }
protected:
  Disk(){
  }
};
}
#endif // DRIVER_DISK_H
