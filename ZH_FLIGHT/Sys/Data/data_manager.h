#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include "os.h"

template <class T>
class DataManager
{
public:
    DataManager();
    void Put(T *data);
    void Get(T *data);
    void GetWithoutDelete(T *data);
    ~DataManager();
private:
    Queue _handler;
};

#endif