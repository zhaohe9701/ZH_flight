#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include "os.h"

template <class T>
class DataManager
{
public:
    DataManager();
    void Push(T *data);
    void Pop(T *data);
    void Update(T *data);
    void Copy(T *data);
    ~DataManager();
private:
    Queue _handler;
};

#endif