#ifndef __IIR_FILTER_H__
#define __IIR_FILTER_H__
#include <stdint.h>

#define IIR_ACC_NSEC 5



typedef struct 
{
    float y[IIR_ACC_NSEC][3];
    float x[IIR_ACC_NSEC + 1][3];
} IirAccParam;


float iir_acc_filter(float x, IirAccParam *p);
void iir_acc_reset(IirAccParam *p);


#endif