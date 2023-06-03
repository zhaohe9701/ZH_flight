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
    virtual void init() = 0;
    virtual uint8_t getId() = 0;
    virtual void getMagData(MagData &data) = 0;
};


#endif