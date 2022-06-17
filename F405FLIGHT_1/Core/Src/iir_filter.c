#include "iir_filter.h"

const float IIR_B[IIR_ACC_NSEC][3] = {
  {
    0.01397117414,              0,              0 
  },
  {
                1,              2,              1 
  },
  {
     0.1121602431,              0,              0 
  },
  {
                1,              1,              0 
  },
  {
                1,              0,              0 
  }
};
const float IIR_A[IIR_ACC_NSEC][3] = {
  {
                1,              0,              0 
  },
  {
                1,   -1.722928882,   0.7788135409 
  },
  {
                1,              0,              0 
  },
  {
                1,  -0.7756795287,              0 
  },
  {
                1,              0,              0 
  }
};

float iir_acc_filter(float in, IirAccParam *p)
{
    uint16_t i;

    p->x[0][0] = in;
    for (i = 0; i < IIR_ACC_NSEC; i++)
    {
        p->y[i][0] = p->x[i][0] * IIR_B[i][0] + p->x[i][1] * IIR_B[i][1] + p->x[i][2] * IIR_B[i][2] - p->y[i][1] * IIR_A[i][1] - p->y[i][2] * IIR_A[i][2];
        p->y[i][0] /= IIR_A[i][0];

        p->y[i][2] = p->y[i][1];
        p->y[i][1] = p->y[i][0];
        p->x[i][2] = p->x[i][1];
        p->x[i][1] = p->x[i][0];

        p->x[i + 1][0] = p->y[i][0];
    }

    return (p->x[IIR_ACC_NSEC][0]);
}
//复位滤波器
void iir_acc_reset(IirAccParam *p)
{
    uint16_t i, j;

    for (i = 0; i < IIR_ACC_NSEC + 1; i++)
    {
        for (j = 0; j < 3; j++)
        {
            p->x[i][j] = 0;
        }
    }

    for (i = 0; i < IIR_ACC_NSEC; i++)
    {
        for (j = 0; j < 3; j++)
        {
            p->y[i][j] = 0;
        }
    }
}