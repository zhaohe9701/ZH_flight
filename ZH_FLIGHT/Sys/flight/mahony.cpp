/*
 * @Description:
 * @Version: 1.0
 * @Author: zhaohe
 * @Date: 2022-09-24 17:52:11
 * @LastEditors: zhaohe
 * @LastEditTime: 2022-09-27 22:18:48
 * @FilePath: \ZH_FLIGHT\Sys\Flight\mahony.cpp
 * Copyright (C) 2022 zhaohe. All rights reserved.
 */
#include "mahony.h"
#include "math_param.h"

void Mahony::Update(AircraftState &actual_state, ImuData &imu_data)
{
    float normalise;
    float ex = 0.0f;
    float ey = 0.0f;
    float ez = 0.0f;
    float gx = imu_data.gyr.x * DEG2RAD;
    float gy = imu_data.gyr.y * DEG2RAD;
    float gz = imu_data.gyr.z * DEG2RAD;
    float ax = imu_data.acc.x;
    float ay = imu_data.acc.y;
    float az = imu_data.acc.z;
    float halfT = 0.5f * _dt;
    float q0_last = 0.0f;
    float q1_last = 0.0f;
    float q2_last = 0.0f;
    float q3_last = 0.0f;

    actual_state.palstance.Set(imu_data.gyr.y, imu_data.gyr.x, imu_data.gyr.z);
    actual_state.acceleration.Set(imu_data.acc.x, imu_data.acc.y, imu_data.acc.z);

#ifdef MAGNETOMETER
    float mx = imu_data.mag.x;
    float my = imu_data.mag.y;
    float mz = imu_data.mag.z;

    float hx = 0.0f;
    float hy = 0.0f;
    float hz = 0.0f;

    float bx = 0.0f;
    float bz = 0.0f;

    float wx = 0.0f;
    float wy = 0.0f;
    float wz = 0.0f;

    if (((mx != 0.0f) || (my != 0.0f) || (mz != 0.0f)) && ((ax != 0.0f) || (ay != 0.0f) || (az != 0.0f)))
    {
        normalise = _InvSqrt(ax * ax + ay * ay + az * az);
        ax *= normalise;
        ay *= normalise;
        az *= normalise;

        normalise = _InvSqrt(mx * mx + my * my + mz * mz);
        mx *= normalise;
        my *= normalise;
        mz *= normalise;

        hx = mx;
        hy = my;
        hz = mz;

        TransformBodyToEarth(hx, hy, hz);

        bx = sqrt(hx * hx + hy * hy);
        bz = hz;
        // bz = mx * _r_mat[2][0] + my * _r_mat[2][1] + mz * _r_mat[2][2];

        // wx = bx * _r_mat[0][0] + bz * _r_mat[2][0];
        // wy = bx * _r_mat[0][1] + bz * _r_mat[2][1];
        // wz = bz * _r_mat[0][2] + bz * _r_mat[2][2];

        wx = bx;
        wy = 0.0f;
        wz = bz;

        TransformEarthToBody(wx, wy, wz);
    
        ex = (ay * _r_mat[2][2] - az * _r_mat[2][1]) + (my * wz - mz * wy);
        ey = (az * _r_mat[2][0] - ax * _r_mat[2][2]) + (mz * wx - mx + wz);
        ez = (ax * _r_mat[2][1] - ay * _r_mat[2][0]) + (mx * wy - my * wx);

        _ex_int += _ki * ex * _dt;
        _ey_int += _ki * ey * _dt;
        _ez_int += _ki * ez * _dt;
    }

#else
    if ((ax != 0.0f) || (ay != 0.0f) || (az != 0.0f))
    {
        normalise = _InvSqrt(ax * ax + ay * ay + az * az);
        ax *= normalise;
        ay *= normalise;
        az *= normalise;

        ex = (ay * _r_mat[2][2] - az * _r_mat[2][1]);
        ey = (az * _r_mat[2][0] - ax * _r_mat[2][2]);
        ez = (ax * _r_mat[2][1] - ay * _r_mat[2][0]);

        _ex_int += _ki * ex * _dt;
        _ey_int += _ki * ey * _dt;
        _ez_int += _ki * ez * _dt;

        gx += _kp * ex + _ex_int;
        gy += _kp * ey + _ey_int;
        gz += _kp * ez + _ez_int;
    }
#endif

    q0_last = _q0;
    q1_last = _q1;
    q2_last = _q2;
    q3_last = _q3;
    _q0 += (-q1_last * gx - q2_last * gy - q3_last * gz) * halfT;
    _q1 += (q0_last * gx + q2_last * gz - q3_last * gy) * halfT;
    _q2 += (q0_last * gy - q1_last * gz + q3_last * gx) * halfT;
    _q3 += (q0_last * gz + q1_last * gy - q2_last * gx) * halfT;

    normalise = _InvSqrt(_q0 * _q0 + _q1 * _q1 + _q2 * _q2 + _q3 * _q3);
    _q0 *= normalise;
    _q1 *= normalise;
    _q2 *= normalise;
    _q3 *= normalise;

    actual_state.quarter.Set(_q1, _q2, _q3, _q0);
    _ComputeRotationMatrix();

    actual_state.euler.PITCH = -asinf(_r_mat[2][0]) * RAD2DEG;
    actual_state.euler.ROLL = atan2f(_r_mat[2][1], _r_mat[2][2]) * RAD2DEG;
    actual_state.euler.YAW = atan2f(_r_mat[1][0], _r_mat[0][0]) * RAD2DEG;
}

float Mahony::_InvSqrt(float x)
{
    float half_x = 0.5f * x;
    float y = x;
    long i = *(long *)&y;

    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (1.5f - (half_x * y * y));

    return y;
}

void Mahony::_ComputeRotationMatrix()
{
    float q1q1 = _q1 * _q1;
    float q2q2 = _q2 * _q2;
    float q3q3 = _q3 * _q3;

    float q0q1 = _q0 * _q1;
    float q0q2 = _q0 * _q2;
    float q0q3 = _q0 * _q3;
    float q1q2 = _q1 * _q2;
    float q1q3 = _q1 * _q3;
    float q2q3 = _q2 * _q3;

    _r_mat[0][0] = 1.0f - 2.0f * q2q2 - 2.0f * q3q3;
    _r_mat[0][1] = 2.0f * (q1q2 + -q0q3);
    _r_mat[0][2] = 2.0f * (q1q3 - -q0q2);

    _r_mat[1][0] = 2.0f * (q1q2 - -q0q3);
    _r_mat[1][1] = 1.0f - 2.0f * q1q1 - 2.0f * q3q3;
    _r_mat[1][2] = 2.0f * (q2q3 + -q0q1);

    _r_mat[2][0] = 2.0f * (q1q3 + -q0q2);
    _r_mat[2][1] = 2.0f * (q2q3 - -q0q1);
    _r_mat[2][2] = 1.0f - 2.0f * q1q1 - 2.0f * q2q2;
}

void Mahony::TransformBodyToEarth(float &x, float &y, float &z)
{
    float ex = 0.0f;
    float ey = 0.0f;
    float ez = 0.0f;

    ex = x * _r_mat[0][0] + y * _r_mat[0][1] + z * _r_mat[0][2];
    ey = x * _r_mat[1][0] + y * _r_mat[1][1] + z * _r_mat[1][2];
    ez = x * _r_mat[2][0] + y * _r_mat[2][1] + z * _r_mat[2][2];

    x = ex;
    y = ey;
    z = ez;
}

void Mahony::TransformEarthToBody(float &x, float &y, float &z)
{
    float bx = 0.0f;
    float by = 0.0f;
    float bz = 0.0f;

    bx = x * _r_mat[0][0] + y * _r_mat[1][0] + z * _r_mat[2][0];
    by = x * _r_mat[0][1] + y * _r_mat[1][1] + z * _r_mat[2][1];
    bz = x * _r_mat[0][2] + y * _r_mat[1][2] + z * _r_mat[2][2];

    x = bx;
    y = by;
    z = bz;
}