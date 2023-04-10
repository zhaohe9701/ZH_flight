#ifndef __MAG_H__
#define __MAG_H__

#include "type.h"

class MagData
{
public:
    Vec3 mag;
};

class Mag
{
public:
    virtual void Init() = 0;
    virtual uint8_t GetId() = 0;
    virtual void GetMagData(MagData &data) = 0;
};


#endif