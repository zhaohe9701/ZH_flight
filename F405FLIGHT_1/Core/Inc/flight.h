#ifndef __FLIGHT_H__
#define __FLIGHT_H__

#include <stdint.h>

#define ATTITUDE_LIMIT 30.0f
#define YAW_PRO 0.02f
#define IDLING 5.0f
#define THROTTLE_SENSTIVITY 0.3f
#define MOTOR1 (motor[0])
#define MOTOR2 (motor[1])
#define MOTOR3 (motor[2])
#define MOTOR4 (motor[3])

typedef struct
{
  float x;
  float y;
  float z;
} FloatVec3;

typedef struct
{
  int16_t x;
  int16_t y;
  int16_t z;
} IntVec3;

typedef struct
{
  FloatVec3 acc;
  FloatVec3 gyro;
  IntVec3 acc_t;
  IntVec3 gyro_t;
} ImuData;

typedef struct
{
  float pitch;
  float roll;
  float yaw;
  float pitch_rate;
  float roll_rate;
  float yaw_rate;
  float throttle;
  float acc_x;
  float acc_y;
  float acc_z;
  float in_yaw;
} State;

typedef struct
{
  float pitch;
  float roll;
  float yaw;
} PidOut;

typedef struct
{
  float m1;
  float m2;
  float m3;
  float m4;
} MotorOut;

enum Command
{
  LOCK_ON = 0x01,
  MANUAL = 0x02,
  AUTO = 0x03,
};

union ParamTrans
{
    uint8_t chr[64];
    float   flo[16];
};

#endif