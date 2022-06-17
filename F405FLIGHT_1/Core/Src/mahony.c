#include "mahony.h"
#include "main.h"
#include "config.h"
#define RAD2DEG 57.29578f
#define DEG2RAD 0.01745329f
//---------------------------------------------------------------------------------------------------
float Kp = 0.4f;	
float Ki = 0.001f;	
float exInt = 0.0f;
float eyInt = 0.0f;
float ezInt = 0.0f;		

float ky = 100.0f;


static float q0 = 1.0f;	
static float q1 = 0.0f;
static float q2 = 0.0f;
static float q3 = 0.0f;	

float dt = 1.0f / MAIN_CYCLE_FREQ;
static float rMat[3][3];

uint8_t is_yaw_init = 0x00;

float invSqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}



void imuComputeRotationMatrix(void)
{
    float q1q1 = q1 * q1;
    float q2q2 = q2 * q2;
    float q3q3 = q3 * q3;

    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q3 = q2 * q3;

    rMat[0][0] = 1.0f - 2.0f * q2q2 - 2.0f * q3q3;
    rMat[0][1] = 2.0f * (q1q2 + -q0q3);
    rMat[0][2] = 2.0f * (q1q3 - -q0q2);

    rMat[1][0] = 2.0f * (q1q2 - -q0q3);
    rMat[1][1] = 1.0f - 2.0f * q1q1 - 2.0f * q3q3;
    rMat[1][2] = 2.0f * (q2q3 + -q0q1);

    rMat[2][0] = 2.0f * (q1q3 + -q0q2);
    rMat[2][1] = 2.0f * (q2q3 - -q0q1);
    rMat[2][2] = 1.0f - 2.0f * q1q1 - 2.0f * q2q2;
}

void MahonyAHRSupdateIMU(ImuData *imu_data,State *actual_state)
{
	float normalise;
	float ex = 0.0, ey = 0.0, ez = 0.0;
	float halfT = 0.5f * dt;
	//Axis3f tempacc = acc;
	actual_state->pitch_rate = imu_data->gyro.y;
	actual_state->roll_rate = imu_data->gyro.x;
	actual_state->yaw_rate = imu_data->gyro.z;
	actual_state->acc_x = imu_data->acc.x;
	actual_state->acc_y = imu_data->acc.y;
	actual_state->acc_z = imu_data->acc.z;
	float gx = imu_data->gyro.x * DEG2RAD;	
	float gy = imu_data->gyro.y * DEG2RAD;
	float gz = imu_data->gyro.z * DEG2RAD;
	float ax = imu_data->acc.x;
	float ay = imu_data->acc.y;
	float az = imu_data->acc.z;

	if((ax != 0.0f) || (ay != 0.0f) || (az != 0.0f))
	{
		normalise = invSqrt(ax * ax + ay * ay + az * az);
		ax *= normalise;
		ay *= normalise;
		az *= normalise;

		ex = (ay * rMat[2][2] - az * rMat[2][1]);
		ey = (az * rMat[2][0] - ax * rMat[2][2]);
		ez = (ax * rMat[2][1] - ay * rMat[2][0]);

		exInt += Ki * ex * dt ;  
		eyInt += Ki * ey * dt ;
		ezInt += Ki * ez * dt ;
		

		gx += Kp * ex + exInt;
		gy += Kp * ey + eyInt;
		gz += Kp * ez + ezInt;
	}

	float q0Last = q0;
	float q1Last = q1;
	float q2Last = q2;
	float q3Last = q3;
	q0 += (-q1Last * gx - q2Last * gy - q3Last * gz) * halfT;
	q1 += ( q0Last * gx + q2Last * gz - q3Last * gy) * halfT;
	q2 += ( q0Last * gy - q1Last * gz + q3Last * gx) * halfT;
	q3 += ( q0Last * gz + q1Last * gy - q2Last * gx) * halfT;
	

	normalise = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
	q0 *= normalise;
	q1 *= normalise;
	q2 *= normalise;
	q3 *= normalise;
	
	imuComputeRotationMatrix();	
	
	actual_state->pitch = -asinf(rMat[2][0]) * RAD2DEG; 
	actual_state->roll = atan2f(rMat[2][1], rMat[2][2]) * RAD2DEG;
	actual_state->yaw = atan2f(rMat[1][0], rMat[0][0]) * RAD2DEG;
}

void MahonyYaw(State *actual_state, uint8_t on)
{
	//usb_printf("%.2f\r\n", actual_state->in_yaw);
	static float last_yaw = 0.0f; 
	if(on == 0x00)
	{
		is_yaw_init = 0x00;
		return;
	}
	
	if(is_yaw_init == 0x00)
	{
		last_yaw = actual_state->in_yaw;
		is_yaw_init = 0x01;
	}

	actual_state->yaw = (1.0f - ky * dt) * last_yaw + ky * dt * actual_state->in_yaw + actual_state->yaw_rate * dt;
	last_yaw = actual_state->yaw;
}
