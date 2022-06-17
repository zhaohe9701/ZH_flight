#include "loop.h"
#include "mpu6000.h"
#include "mahony.h"
#include "pid.h"
#include "dshot.h"
#include "upper_computer.h"
#include "system.h"
#include "param_service.h"
#include "filter.h"
//#include "iir_filter.h"
#include "config.h"
ImuData imu_data;
ImuData temp_imu_data;
extern State actual_state;
extern State expect_state;
extern uint16_t ibus_channel[18];
extern union ParamTrans upper_command_raw;
BicycloPidParam pitch_param;
BicycloPidParam roll_param;
BicycloPidParam yaw_param;

volatile enum Mutex remote_cmd_mutex = UNLOCK;
volatile enum Mutex upper_cmd_mutex = UNLOCK;
enum Command command = LOCK_ON;
enum Command last_command = LOCK_ON;
uint8_t is_unlock_motor = 0;
uint8_t is_cali_imu = 0;
PidOut pid_out;

//extern TIM_HandleTypeDef SENSOR_LOOP_TIMER;

uint16_t motor[4] = {0};
float attitude_senstivity = ATTITUDE_LIMIT / 500.0f;

static float Limit0(float value)
{
    if(value > 2047.0f)
    {
        return 2047.0f;
    }
    else if(value < 0.0f)
    {
        return 0.0f;
    }
    else
    {
        return value;
    }
}

static float Limit(float value, float limit)
{
    if(value > limit)
    {
        return limit;
    }
    else if(value < -limit)
    {
        return -limit;
    }
    else
    {
        return value;
    }
}

void Unlock_Motor()
{
    static uint16_t time = 0;
    //static uint16_t motor_value = 0;
    if(time <= 1500)
    {
        MOTOR1 = 0;
        MOTOR2 = 0;
        MOTOR3 = 0;
        MOTOR4 = 0;
    }
    else
    {
        is_unlock_motor = 1;
        MOTOR1 = 0;
        MOTOR2 = 0;
        MOTOR3 = 0;
        MOTOR4 = 0;
        time = 0;
        return;
    }
    time++;
}


void Update_expect_State_And_Command_By_Remote(uint8_t _time)
{
    if(_time % 2 != 0)
    {
        return;
    }
    static uint16_t loss_remote_time = 0;
    static uint8_t last_remote_ind = 0;
    extern uint8_t remote_ind;
    static uint8_t timeout_lock = 0x00;
    if(last_remote_ind == remote_ind)
    {
        loss_remote_time++;
    }
    else
    {
        last_remote_ind = remote_ind;
        loss_remote_time = 0;
    }  
    if(loss_remote_time >= 500)
    {
        loss_remote_time = 500;
        timeout_lock = 0x01;

    }
    static int16_t pitch_raw = 0, roll_raw = 0, yaw_raw = 0, throttle_raw = 0;
    static uint16_t swa_raw = 0;
    static uint16_t swc_raw = 0;
    last_command = command;
    if(remote_cmd_mutex == UNLOCK)
    {
        remote_cmd_mutex = LOCK;
        pitch_raw       = ibus_channel[1];
        roll_raw        = ibus_channel[0];
        yaw_raw         = ibus_channel[3];
        throttle_raw    = ibus_channel[2];
        swa_raw         = ibus_channel[4];
        swc_raw         = ibus_channel[5];
        remote_cmd_mutex = UNLOCK;
    }
    if(swc_raw == 1000 || (command == LOCK_ON && throttle_raw > 1010))
    {
        command = LOCK_ON;
        //usb_printf("%d\r\n",throttle_raw);
    }
    else if(swc_raw == 1500)
    {
        command = MANUAL;
        timeout_lock = 0x00;
        //usb_printf("MANUAL\r\n");
    }
    else if(swc_raw == 2000)
    {
        command = AUTO;
        timeout_lock = 0x00;
        //usb_printf("AUTO\r\n");
    }
    else;
    if(timeout_lock == 0x01)
    {
        command = LOCK_ON;
    }
    if(command != AUTO)
    {
        expect_state.pitch      =  (float)(pitch_raw     - 1500) * attitude_senstivity;
        expect_state.roll       =  (float)(roll_raw      - 1500) * attitude_senstivity;
        float yaw_rate          =  (float)(yaw_raw       - 1500) * attitude_senstivity;
        expect_state.yaw        -=  (yaw_rate * YAW_PRO);
        if(expect_state.yaw > 180.0f)
        {
            expect_state.yaw -= 360.0f;
        }
        else if(expect_state.yaw < -180.0f)
        {
            expect_state.yaw += 360.0f;
        }
        else;
        expect_state.throttle   =   IDLING + (float)(throttle_raw  - 1000) * (0.1f - IDLING * 0.001f) * THROTTLE_SENSTIVITY;
    }

    if(command == LOCK_ON && swa_raw == 2000)
    {
        is_cali_imu = 1;
    }
    //usb_printf("%.2f,%.2f,%.2f,%.2f\t\n", expect_state.pitch, expect_state.roll, expect_state.yaw, expect_state.throttle);

}

void Update_expect_State_By_Upper_Computer(uint8_t _time)
{
    if(_time % 2 != 0)
    {
        return;
    }
    if(upper_cmd_mutex == UNLOCK)
    {   
        upper_cmd_mutex = LOCK;
        expect_state.pitch = upper_command_raw.flo[1];
        expect_state.roll = upper_command_raw.flo[2];
        expect_state.yaw = upper_command_raw.flo[3];
        expect_state.throttle = upper_command_raw.flo[4];    
        actual_state.in_yaw = upper_command_raw.flo[5];   
        upper_cmd_mutex = UNLOCK;
    }
    expect_state.throttle = IDLING + Limit(expect_state.throttle, 100.0f) * (1.0f - IDLING * 0.01f) * THROTTLE_SENSTIVITY;
    expect_state.pitch = Limit(expect_state.pitch, ATTITUDE_LIMIT);
    expect_state.roll = Limit(expect_state.roll, ATTITUDE_LIMIT);
    expect_state.yaw = Limit(expect_state.yaw, 180.0f);

    if(expect_state.yaw > 180.0f)
    {
        expect_state.yaw -= 360.0f;
    }
    else if(expect_state.yaw < -180.0f)
    {
        expect_state.yaw += 360.0f;
    }
    else;
}

void Update_Actual_State(uint8_t _time)
{
    if(_time % 1 != 0)
    {
        return;
    }
    
    //MPU_Get_Acc_And_Gyro(&imu_data.acc.x, &imu_data.acc.y, &imu_data.acc.z, &imu_data.gyro.x, &imu_data.gyro.y, &imu_data.gyro.z);
    //__HAL_TIM_DISABLE_IT(&SENSOR_LOOP_TIMER, TIM_IT_UPDATE);
    HAL_NVIC_DisableIRQ(GYRO_EXTI_IRQ);
    imu_data.acc.x = temp_imu_data.acc.x;
    imu_data.acc.y = temp_imu_data.acc.y;
    imu_data.acc.z = temp_imu_data.acc.z;
    imu_data.gyro.x = temp_imu_data.gyro.x;
    imu_data.gyro.y = temp_imu_data.gyro.y;
    imu_data.gyro.z = temp_imu_data.gyro.z;
    HAL_NVIC_EnableIRQ(GYRO_EXTI_IRQ);
    //__HAL_TIM_ENABLE_IT(&SENSOR_LOOP_TIMER, TIM_IT_UPDATE);

    MahonyAHRSupdateIMU(&imu_data,&actual_state);
    if(command == AUTO)
    {
        MahonyYaw(&actual_state, 0x01);
    }
    else
    {
        MahonyYaw(&actual_state, 0x00);
    }
    //usb_printf("%.2f\t%.2f\t%.2f\r\n", imu_data.gyro.x, imu_data.gyro.y, imu_data.gyro.z);
    //usb_printf("%.2f\t%.2f\t%.2f\r\n", actual_state.pitch, actual_state.roll, actual_state.yaw);
}

void Attitude_Control(uint8_t _time)
{
    if(_time % 1 != 0)
    {
        return;
    }
    pid_out.pitch = Pitch_Bicyclo_Pid(&actual_state, &expect_state, &pitch_param);
    pid_out.roll = Roll_Bicyclo_Pid(&actual_state, &expect_state, &roll_param);
    pid_out.yaw = Yaw_Bicyclo_Pid(&actual_state, &expect_state, &yaw_param);
    //usb_printf("%.2f\t%.2f\t%.2f\r\n", pid_out.pitch ,pid_out.roll, pid_out.yaw);
}

void Power_Distribution(uint8_t _time)
{
    if(_time % 1 != 0)
    {
        return;
    }
    MOTOR1 = (uint16_t)(Limit0((expect_state.throttle + pid_out.pitch - pid_out.roll + pid_out.yaw) * 20.47f));
    MOTOR2 = (uint16_t)(Limit0((expect_state.throttle - pid_out.pitch - pid_out.roll - pid_out.yaw) * 20.47f));
    MOTOR3 = (uint16_t)(Limit0((expect_state.throttle + pid_out.pitch + pid_out.roll - pid_out.yaw) * 20.47f));
    MOTOR4 = (uint16_t)(Limit0((expect_state.throttle - pid_out.pitch + pid_out.roll + pid_out.yaw) * 20.47f));

}

void Motor_Control(uint8_t _time)
{
    if(_time % 1 != 0)
    {
        return;
    }
    SetMotorValue(motor);
}

void Main_Loop()
{
    static uint8_t _time = 0;
    if(_time > 10)
    {
        _time = 0;
    }
    Update_expect_State_And_Command_By_Remote(_time);
    
    if(is_unlock_motor == 0)
    {
        Unlock_Motor();
    }

    Update_Actual_State(_time);
    if(last_command == AUTO && command == MANUAL)
    {
        expect_state.yaw = actual_state.yaw;
    }
    if(command == AUTO)
    {
        Update_expect_State_By_Upper_Computer(_time);
    }
    //usb_printf("%.2f %.2f %.2f %.2f\r\n", expect_state.pitch, expect_state.roll, expect_state.yaw, expect_state.throttle);
    


    //usb_printf("%.2f\t%.2f\t%.2f\r\n", actual_state.pitch_rate ,actual_state.roll_rate, actual_state.yaw_rate);
    if((command == MANUAL || command == AUTO) && is_unlock_motor == 1)
    {
        Attitude_Control(_time);

        Power_Distribution(_time);

        
    }



    if(command == LOCK_ON)
    {
        union ParamTrans param;

        MOTOR1 = 0;
        MOTOR2 = 0;
        MOTOR3 = 0;
        MOTOR4 = 0;
        expect_state.yaw = actual_state.yaw;
        if(Usb_Receive(param.chr))
        {
            //usb_printf("%x %x %.2f %.2f %.2f\r\n", param.chr[0], param.chr[1], param.flo[1], param.flo[2], param.flo[3]);
            Param_Service(&param);
        }
    }
    
    Motor_Control(_time);


    if(command == LOCK_ON && is_cali_imu == 1)
    {
        if(!Calibrate_Imu());
        else 
        {
            //usb_printf("Calibrate IMU finish!\r\n");
            is_cali_imu = 0;
        }
    }


    //Send_State_To_Upper_Computer(&actual_state, 5);
    _time++;
}


static lpf2pData accLpf[3];
static lpf2pData gyroLpf[3];
//static IirAccParam iir_acc_param[3];


void Filer_Init()
{
    for (uint8_t i = 0; i < 3; i++)
	{
		lpf2pInit(&gyroLpf[i], SENSOR_CYCLE_FREQ , GYRO_LPF_CUTOFF_FREQ);
        //iir_acc_reset(&iir_acc_param[i]);
		lpf2pInit(&accLpf[i],  SENSOR_CYCLE_FREQ , ACCEL_LPF_CUTOFF_FREQ);
	}
}
void Sensor_Loop()
{
    
    MPU_Get_Acc(&temp_imu_data.acc.x, &temp_imu_data.acc.y, &temp_imu_data.acc.z);
    MPU_Get_Gyro(&temp_imu_data.gyro.x, &temp_imu_data.gyro.y, &temp_imu_data.gyro.z);
    temp_imu_data.acc.x = lpf2pApply(&accLpf[0], temp_imu_data.acc.x);
    temp_imu_data.acc.y = lpf2pApply(&accLpf[1], temp_imu_data.acc.y);
    temp_imu_data.acc.z = lpf2pApply(&accLpf[2], temp_imu_data.acc.z);
    // temp_imu_data.acc.x = iir_acc_filter(temp_imu_data.acc.x, &iir_acc_param[0]);
    // temp_imu_data.acc.y = iir_acc_filter(temp_imu_data.acc.y, &iir_acc_param[1]);
    // temp_imu_data.acc.z = iir_acc_filter(temp_imu_data.acc.z, &iir_acc_param[2]);
    temp_imu_data.gyro.x = lpf2pApply(&gyroLpf[0], temp_imu_data.gyro.x);
    temp_imu_data.gyro.y = lpf2pApply(&gyroLpf[1], temp_imu_data.gyro.y);
    temp_imu_data.gyro.z = lpf2pApply(&gyroLpf[2], temp_imu_data.gyro.z);
    //usb_printf("%.2f\t%.2f\t%.2f\r\n", temp_imu_data.gyro.x, temp_imu_data.gyro.y, temp_imu_data.gyro.z);
    //usb_printf("%.4f\r\n", temp_imu_data.gyro.x);
}