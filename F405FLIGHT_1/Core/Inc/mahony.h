#ifndef __MAHONY_H__
#define __MAHONY_H__
#include "math.h"
#include "mpu6000.h"
#include "flight.h"

void MahonyAHRSupdateIMU(ImuData *imu_data, State *actual_state);
void MahonyYaw(State *actual_state, uint8_t on);
#endif