#include "mpu6000.h"
#include "stdio.h"
#include "config.h"
#include "iir_filter.h"
#define MPU6000DelayMs(_nms) HAL_Delay(_nms)

#define MPU_SPI_CS_LOW() HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_RESET)
#define MPU_SPI_CS_HIGH() HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_SET)

#define DATABUF_SIZE 16

extern SPI_HandleTypeDef IMU_SPI;
static float acc_sensitivity = 0.488f;
static float gyro_sensitivity = 0.00763f; //陀螺仪的最小分辨率
static uint16_t acc_lsb = 2048;
static int16_t bias_acc_x = 40;
static int16_t bias_acc_y = 0;
static int16_t bias_acc_z = 170;
static int16_t bias_gyro_x = -12;
static int16_t bias_gyro_y = 23;
static int16_t bias_gyro_z = -10;

//static IirParam gx_iir, gy_iir, gz_iir, ax_iir, ay_iir, az_iir;
//static uint8_t dataBuf[DATABUF_SIZE] = {0};

/*  MPU使能  */
void mpu_enable()
{
    HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_RESET);
}

/*  MPU关闭  */
void mpu_disable()
{
    HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_SET);
}

/*  SPI写字节  */
static uint8_t spi_wr_byte(SPI_HandleTypeDef *hspi, uint8_t byte)
{
    uint8_t feedback = 0;
    // wait SPI serial free
    while (HAL_SPI_GetState(hspi) == HAL_SPI_STATE_BUSY_TX_RX)
        ;

    if (HAL_SPI_TransmitReceive(hspi, &byte, &feedback, 1, 0x01f4) != HAL_OK)
    {
        return 0xff;
    }
    return feedback;
}

/*  SPI写  */
// static void spi_w_bytes(SPI_HandleTypeDef *hspi, uint8_t address, uint8_t *bytes, uint16_t num)
// {
//     mpu_enable();
//     uint8_t i;
//     spi_wr_byte(hspi, address);
//     for (i = 0; i < num; i++)
//         spi_wr_byte(hspi, bytes[i]);
//     mpu_disable();
// }

static void spi_w_byte(SPI_HandleTypeDef *hspi, uint8_t address, uint8_t byte)
{
    mpu_enable();

    spi_wr_byte(hspi, address);
    spi_wr_byte(hspi, byte);
    mpu_disable();
}

/*  SPI读  */
static void spi_r_bytes(SPI_HandleTypeDef *hspi, uint8_t address, uint8_t num, uint8_t *dataBuf)
{
    uint8_t _address = address | 0x80;

    mpu_enable();
    HAL_SPI_Transmit(hspi, &_address, 1, 0x01f4);
    HAL_SPI_Receive(hspi, dataBuf, num, 0x01f4);
    mpu_disable();
}

/*  读单字节  */
static uint8_t mpu_read_reg(uint8_t reg)
{
    uint8_t regval = 0;
    spi_r_bytes(&IMU_SPI, reg, 1, &regval);
    return regval;
}

/*  连续读  */
static void mpu_read_regs(uint8_t reg, uint16_t len, uint8_t *data)
{
    spi_r_bytes(&IMU_SPI, reg, len, data);
}

/*  写寄存器  */
static void mpu_write_reg(uint8_t reg, uint8_t value)
{
    spi_w_byte(&IMU_SPI, reg, value);
}


uint8_t get_lpf_value(const uint8_t frq)
{
    uint8_t lpf = 0x01;
    switch (frq)
    {
    case GODR_1000Hz:
        lpf = 0x01;
        break;
    case GODR_500Hz:
        lpf = 0x01;
        break;
    case GODR_100Hz:
        lpf = 0x02;
        break;
    default:
        break;
    }
    return lpf;
}

float get_acc_LSB(uint8_t afs)
{
    float lsb = 4096.0f;
    switch (afs)
    {
    case AFS_2G:
        lsb = 16384.0f;
        break;
    
    case AFS_4G:
        lsb = 8192.0f;
        break;
    
    case AFS_8G:
        lsb = 4096.0f;
        break;
    
    case AFS_16G:
        lsb = 2048.0f;
        break;

    default:
        break;
    }
    return lsb;
}

float get_gyro_LSB(uint8_t afs)
{
    float lsb = 16.4f;
    switch (afs)
    {
    case GFS_250DPS:
        lsb = 131.0f;
        break;
    
    case GFS_500DPS:
        lsb = 65.5f;
        break;
    
    case GFS_1000DPS:
        lsb = 32.8f;
        break;
    
    case GFS_2000DPS:
        lsb = 16.4f;
        break;

    default:
        break;
    }
    return lsb;
}

uint8_t MPU_Init(void)
{
    uint8_t res;
    MPU6000DelayMs(100);
    mpu_write_reg(MPU_PWR_MGMT1_REG, 0X80);                     //复位MPU6050
    MPU6000DelayMs(100);
    mpu_write_reg(MPU_SIGPATH_RST_REG, 0X04 | 0x02 | 0x01);
    MPU6000DelayMs(100);
    mpu_write_reg(MPU_PWR_MGMT1_REG, 0X03);                     //复位MPU6050
    MPU6000DelayMs(15);
    mpu_write_reg(MPU_USER_CTRL_REG, 0x10);                     //屏蔽IIC接口
    MPU6000DelayMs(15);
    mpu_write_reg(MPU_PWR_MGMT2_REG, 0X00);                     //加速度与陀螺仪都工作
    MPU6000DelayMs(15);
    mpu_write_reg(MPU_SAMPLE_RATE_REG, GODR_1000Hz);            //设置采样率1000Hz
    MPU6000DelayMs(15);
    mpu_write_reg(MPU_CFG_REG, get_lpf_value(GODR_1000Hz));     //设置低通滤波器
    MPU6000DelayMs(15);                               
    // mpu_write_reg(MPU_SAMPLE_RATE_REG, 0x07);            //设置采样率1000Hz
    // MPU6000DelayMs(15);
    // mpu_write_reg(MPU_CFG_REG, 0x00);     //设置低通滤波器
    // MPU6000DelayMs(15);         
    mpu_write_reg(MPU_GYRO_CFG_REG, GFS_2000DPS);               //陀螺仪传感器量程
    MPU6000DelayMs(15);                           
    mpu_write_reg(MPU_ACCEL_CFG_REG, AFS_16G);                   //加速度传感器量程
    MPU6000DelayMs(15);
    gyro_sensitivity = 1.0f / (get_gyro_LSB(GFS_2000DPS));

    acc_sensitivity = 1.0f / (get_acc_LSB(AFS_16G)) * 1000.0f;

    acc_lsb = get_acc_LSB(AFS_16G);
    mpu_write_reg(MPU_INTBP_CFG_REG, 0x90);         
    MPU6000DelayMs(10);
    mpu_write_reg(MPU_INT_EN_REG, DATA_RDY_EN);    //使能数据就绪中断
    MPU6000DelayMs(10);
    res=mpu_read_reg(MPU_DEVICE_ID_REG);

   
    if (res == MPU_ADDR) //器件ID正确
    {
        //iir_reset(&gx_iir);
        //iir_reset(&gy_iir);
        //iir_reset(&gz_iir);
        //iir_reset(&ax_iir);
        //iir_reset(&ay_iir);
        //iir_reset(&az_iir);
        return 0;
    }
    else
    {
        return 1;
    }
}


/*  获取芯片编号  */

uint8_t Get_MPU_Id()
{
    return mpu_read_reg(MPU_DEVICE_ID_REG);
}



//得到温度值
//返回值:温度值(扩大了100倍)
float MPU_Get_Temperature(void)
{
    uint8_t buf[2];
    short raw;
    float temp;
    mpu_read_regs(MPU_TEMP_OUTH_REG, 2, buf);
    raw = (buf[0] << 8) | buf[1];
    temp = (36.53 + ((double)raw) / 340) * 100;
    //  temp = (long)((35 + (raw / 340)) * 65536L);
    return temp / 100.0f;
}

// void MPU_Get_Gyro(float *gx, float *gy, float *gz)
// {
//     uint8_t buf[6];
//     mpu_read_regs(MPU_GYRO_XOUTH_REG, 6, buf);
//     uint16_t gx_raw, gy_raw, gz_raw;

//     gx_raw = ((uint16_t)buf[0] << 8) | buf[1];
//     gy_raw = ((uint16_t)buf[2] << 8) | buf[3];
//     gz_raw = ((uint16_t)buf[4] << 8) | buf[5];
    
//     int16_t gx_lpf = ((int16_t)(gx_raw) - bias_gyro_x);
//     int16_t gy_lpf = ((int16_t)(gy_raw) - bias_gyro_y);
//     int16_t gz_lpf = ((int16_t)(gz_raw) - bias_gyro_z);

//     gx_lpf = iir_filter(gx_lpf, &gx_iir);
//     gy_lpf = iir_filter(gy_lpf, &gy_iir);
//     gz_lpf = iir_filter(gz_lpf, &gz_iir);

//     *gx = (float)gx_lpf * gyro_sensitivity;
// 	*gy = (float)gy_lpf * gyro_sensitivity;
// 	*gz = (float)gz_lpf * gyro_sensitivity;
// }

// void MPU_Get_Acc(float *ax, float *ay, float *az)
// {
//     uint8_t buf[6];
//     mpu_read_regs(MPU_ACCEL_XOUTH_REG, 6, buf);
//     uint16_t ax_raw, ay_raw, az_raw;

//     ax_raw = ((uint16_t)buf[0] << 8) | buf[1];
//     ay_raw = ((uint16_t)buf[2] << 8) | buf[3];
//     az_raw = ((uint16_t)buf[4] << 8) | buf[5];

//     int16_t ax_lpf = ((int16_t)(ax_raw) - bias_acc_x);
//     int16_t ay_lpf = ((int16_t)(ay_raw) - bias_acc_y);
//     int16_t az_lpf = ((int16_t)(az_raw) - bias_acc_z);

//     ax_lpf = iir_filter(ax_lpf, &ax_iir);
//     ay_lpf = iir_filter(ay_lpf, &ay_iir);
//     az_lpf = iir_filter(az_lpf, &az_iir);

//     *ax = (float)ax_lpf * acc_sensitivity;
// 	*ay = (float)ay_lpf * acc_sensitivity;
// 	*az = (float)az_lpf * acc_sensitivity;
// }

void MPU_Get_Gyro(float *gx, float *gy, float *gz)
{
    uint8_t buf[6];
    mpu_read_regs(MPU_GYRO_XOUTH_REG, 6, buf);
    uint16_t gx_raw, gy_raw, gz_raw;

    gx_raw = ((uint16_t)buf[0] << 8) | buf[1];
    gy_raw = ((uint16_t)buf[2] << 8) | buf[3];
    gz_raw = ((uint16_t)buf[4] << 8) | buf[5];
    *gx = (float)((int16_t)(gx_raw) - bias_gyro_x) * gyro_sensitivity;
	*gy = (float)((int16_t)(gy_raw) - bias_gyro_y) * gyro_sensitivity;
	*gz = (float)((int16_t)(gz_raw) - bias_gyro_z) * gyro_sensitivity;
}

void MPU_Get_Acc(float *ax, float *ay, float *az)
{
    uint8_t buf[6];
    mpu_read_regs(MPU_ACCEL_XOUTH_REG, 6, buf);
    uint16_t ax_raw, ay_raw, az_raw;

    ax_raw = ((uint16_t)buf[0] << 8) | buf[1];
    ay_raw = ((uint16_t)buf[2] << 8) | buf[3];
    az_raw = ((uint16_t)buf[4] << 8) | buf[5];
    *ax = (float)((int16_t)(ax_raw) - bias_acc_x) * acc_sensitivity;
	*ay = (float)((int16_t)(ay_raw) - bias_acc_y) * acc_sensitivity;
	*az = (float)((int16_t)(az_raw) - bias_acc_z) * acc_sensitivity;
}

void MPU_Get_Gyro_Raw(int16_t *gx, int16_t *gy, int16_t *gz)
{
    uint8_t buf[6];
    mpu_read_regs(MPU_GYRO_XOUTH_REG, 6, buf);
    uint16_t gx_raw, gy_raw, gz_raw;

    gx_raw = ((uint16_t)buf[0] << 8) | buf[1];
    gy_raw = ((uint16_t)buf[2] << 8) | buf[3];
    gz_raw = ((uint16_t)buf[4] << 8) | buf[5];
    *gx = (int16_t)(gx_raw);
	*gy = (int16_t)(gy_raw);
	*gz = (int16_t)(gz_raw);
}

void MPU_Get_Acc_Raw(int16_t *ax, int16_t *ay, int16_t *az)
{
    uint8_t buf[6];
    mpu_read_regs(MPU_ACCEL_XOUTH_REG, 6, buf);
    uint16_t ax_raw, ay_raw, az_raw;

    ax_raw = ((uint16_t)buf[0] << 8) | buf[1];
    ay_raw = ((uint16_t)buf[2] << 8) | buf[3];
    az_raw = ((uint16_t)buf[4] << 8) | buf[5];
    *ax = (int16_t)(ax_raw);
	*ay = (int16_t)(ay_raw);
	*az = (int16_t)(az_raw);
}

uint8_t Calibrate_Imu()
{
    static int16_t time = 0;
    static int32_t sum_acc_x = 0, sum_acc_y = 0, sum_acc_z = 0;
    static int32_t sum_gyr_x = 0, sum_gyr_y = 0, sum_gyr_z = 0;
    int16_t acc_x, acc_y, acc_z, gyr_x, gyr_y, gyr_z;
    if(time < CALIBRATE_TIME)
    {
        MPU_Get_Gyro_Raw(&gyr_x, &gyr_y, &gyr_z);
        MPU_Get_Acc_Raw(&acc_x, &acc_y, &acc_z);
        //usb_printf("acc:\t%d\t%d\t%d\t\r\n", gyr_x,gyr_y,gyr_z);
        //usb_printf("acc:\t%d\t%d\t%d\t\r\n", acc_x,acc_y,acc_z);
        sum_acc_x += acc_x;
        sum_acc_y += acc_y;
        sum_acc_z += (acc_z - acc_lsb);
        sum_gyr_x += gyr_x;
        sum_gyr_y += gyr_y;
        sum_gyr_z += gyr_z;
        time++;
        return 0;
    }
    else
    {
        
        bias_acc_x = sum_acc_x / (int32_t)time;
        bias_acc_y = sum_acc_y / (int32_t)time;
        bias_acc_z = sum_acc_z / (int32_t)time;

        bias_gyro_x = sum_gyr_x / (int32_t)time;
        bias_gyro_y = sum_gyr_y / (int32_t)time;
        bias_gyro_z = sum_gyr_z / (int32_t)time;
        //usb_printf("acc:", sum_acc_x,sum_acc_y,sum_acc_z);
        usb_printf("gyr:\t%d\t%d\t%d\tacc:%d\t%d\t%d\t\r\n", bias_gyro_x,bias_gyro_y,bias_gyro_z, bias_acc_x,bias_acc_y,bias_acc_z);
        sum_acc_x = 0;
        sum_acc_y = 0;
        sum_acc_z = 0;
        sum_gyr_x = 0;
        sum_gyr_y = 0;
        sum_gyr_z = 0;

        time = 0;
        //usb_printf("acc:\t%d\t%d\t%d\t%d\t%d\t%d\r\n", bias_acc_x,bias_acc_y,bias_acc_z, bias_gyro_x,bias_gyro_y,bias_gyro_z);
        //usb_printf("gyro:\t%d\t%d\t%d\t\r\n", bias_gyro_x,bias_gyro_y,bias_gyro_z);
        return 1;
    }




}
