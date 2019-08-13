#ifndef DRIVER_ADCARD16_H
#define DRIVER_ADCARD16_H

#include "driver_abstract_adcard.h"
namespace driver {
class ADCard16 : public AbstractADCard<quint16>
{
public:
  static ADCard16 *instance();

protected:
  explicit ADCard16(){}
};
}
#endif // ADCARD16_H
