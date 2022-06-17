#include "icm42605.h"
#include "config.h"
static float accSensitivity = 0.244f; //加速度的最小分辨率 mg/LSB
static float gyroSensitivity = 0.0305f; //陀螺仪的最小分辨率

extern SPI_HandleTypeDef IMU_SPI;

/*ICM42605使用的ms级延时函数，须由用户提供。*/
#define ICM42605DelayMs(_nms) HAL_Delay(_nms)
#define DATABUF_SIZ 16

static uint8_t dataBuf[DATABUF_SIZ] = {0};

/*******************************************************************************
* 名    称： mpu_select()
* 功    能： spi从机使能
* 入口参数： 无
* 出口参数： 无
* 作　　者： zhaohe
* 创建日期： 2022-01-10
* 修    改：
* 修改日期：
* 备    注： 
*******************************************************************************/
void mpu_select()
{
    HAL_GPIO_WritePin(ICM42605_CS_GPIO, ICM42605_CS_PIN, GPIO_PIN_RESET);
}

/*******************************************************************************
* 名    称： mpu_deselect()
* 功    能： spi从机关闭
* 入口参数： 无
* 出口参数： 无
* 作　　者： zhaohe
* 创建日期： 2022-01-10
* 修    改：
* 修改日期：
* 备    注： 
*******************************************************************************/
void mpu_deselect()
{
    HAL_GPIO_WritePin(ICM42605_CS_GPIO, ICM42605_CS_PIN, GPIO_PIN_SET);
}

/*
 * @brief   write several bytes though spi
 * @param   address: address of the first reg
 * @param   bytes: number of bytes to write
 * @param   num: number of bytes
 * */

static uint8_t spi_wr_byte(SPI_HandleTypeDef *hspi, uint8_t byte) {
	uint8_t feedback = 0;

	// wait SPI serial free
	while (HAL_SPI_GetState(hspi) == HAL_SPI_STATE_BUSY_TX_RX)
		;

	if (HAL_SPI_TransmitReceive(hspi, &byte, &feedback, 1, 0x01f4) != HAL_OK) {
		return 0xff;
	}

	return feedback;
}
/*
 * @brief   write several bytes though spi
 * @param   address: address of the first reg
 * @param   bytes: number of bytes to write
 * @param   num: number of bytes
 * */
static void spi_w_bytes(SPI_HandleTypeDef *hspi, uint8_t address,
		uint8_t *bytes, uint16_t num) {
	mpu_select();
	uint8_t i;
	spi_wr_byte(hspi, address);
	for (i = 0; i < num; i++)
		spi_wr_byte(hspi, bytes[i]);

	mpu_deselect();
}


/*
 * @brief   read several bytes though spi
 * @param   address: address of the first reg
 * @param   num: number of bytes to read, number < DATABUF_SIZ
 * @return  data read array
 * */
static void spi_r_bytes(SPI_HandleTypeDef *hspi, uint8_t address, uint8_t num)
{
    uint8_t _address = address | 0x80;

    mpu_select();

    // may be can use HAL_SPI_TransmitReceive()
    HAL_SPI_Transmit(hspi, &_address, 1, 0x01f4);
    HAL_SPI_Receive(hspi, dataBuf, num, 0x01f4); // store read data to dataBuf

    mpu_deselect();
}


/*******************************************************************************
* 名    称： icm42605_read_reg
* 功    能： 读取单个寄存器的值
* 入口参数： reg: 寄存器地址
* 出口参数： 当前寄存器地址的值
* 作　　者： Roger-WY.
* 创建日期： 2021-05-21
* 修    改：
* 修改日期：
* 备    注： 使用SPI读取寄存器时要注意:最高位为读写位，详见datasheet page50.
*******************************************************************************/
static uint8_t icm42605_read_reg(uint8_t reg)
{
    uint8_t regval = 0;
    spi_r_bytes(&IMU_SPI, reg, 1);
    regval = dataBuf[0];
    return regval;
}

/*******************************************************************************
* 名    称： icm42605_read_regs
* 功    能： 连续读取多个寄存器的值
* 入口参数： reg: 起始寄存器地址 *buf数据指针,uint16_t len长度
* 出口参数： 无
* 作　　者： Roger-WY.
* 创建日期： 2021-05-21
* 修    改：
* 修改日期：
* 备    注： 使用SPI读取寄存器时要注意:最高位为读写位，详见datasheet page50.
*******************************************************************************/
static void icm42605_read_regs(uint8_t reg, uint16_t len)
{

    spi_r_bytes(&IMU_SPI, reg, len);
}

/*******************************************************************************
* 名    称： icm42605_write_reg
* 功    能： 向单个寄存器写数据
* 入口参数： reg: 寄存器地址 value:数据
* 出口参数： 0
* 作　　者： Roger-WY.
* 创建日期： 2021-05-21
* 修    改：
* 修改日期：
* 备    注： 使用SPI读取寄存器时要注意:最高位为读写位，详见datasheet page50.
*******************************************************************************/
static void icm42605_write_reg(uint8_t reg, uint8_t value)
{
    spi_w_bytes(&IMU_SPI, reg, &value, 1);
}

/*******************************************************************************
* 名    称： Icm42605GetAres
* 功    能： 设置加速度量程
* 入口参数： Ascale: 量程
* 出口参数： 分辨率
* 作　　者： zhaohe
* 创建日期： 2022-01-10
* 修    改：
* 修改日期：
* 备    注： 
*******************************************************************************/
float Icm42605GetAres(uint8_t Ascale)
{
    switch (Ascale)
    {
    // Possible accelerometer scales (and their register bit settings) are:
    // 2 Gs (00), 4 Gs (01), 8 Gs (10), and 16 Gs  (11).
    case AFS_2G:
        accSensitivity = 2000 / 32768.0f;
        break;
    case AFS_4G:
        accSensitivity = 4000 / 32768.0f;
        break;
    case AFS_8G:
        accSensitivity = 8000 / 32768.0f;
        break;
    case AFS_16G:
        accSensitivity = 16000 / 32768.0f;
        break;
    }

    return accSensitivity;
}

/*******************************************************************************
* 名    称： Icm42605GetGres
* 功    能： 设置陀螺仪量程
* 入口参数： Ascale: 量程
* 出口参数： 分辨率
* 作　　者： zhaohe
* 创建日期： 2022-01-10
* 修    改：
* 修改日期：
* 备    注： 
*******************************************************************************/
float Icm42605GetGres(uint8_t Gscale)
{
    switch (Gscale)
    {
    case GFS_15_125DPS:
        gyroSensitivity = 15.125f / 32768.0f;
        break;
    case GFS_31_25DPS:
        gyroSensitivity = 31.25f / 32768.0f;
        break;
    case GFS_62_5DPS:
        gyroSensitivity = 62.5f / 32768.0f;
        break;
    case GFS_125DPS:
        gyroSensitivity = 125.0f / 32768.0f;
        break;
    case GFS_250DPS:
        gyroSensitivity = 250.0f / 32768.0f;
        break;
    case GFS_500DPS:
        gyroSensitivity = 500.0f / 32768.0f;
        break;
    case GFS_1000DPS:
        gyroSensitivity = 1000.0f / 32768.0f;
        break;
    case GFS_2000DPS:
        gyroSensitivity = 2000.0f / 32768.0f;
        break;
    }
    return gyroSensitivity;
}
/*******************************************************************************
* 名    称： IcmSetBias
* 功    能： 读取Icm42605 内部传感器温度
* 入口参数： 无
* 出口参数： 无
* 作　　者： zhaohe
* 创建日期： 2022-01-13
* 修    改：
* 修改日期：
* 备    注： datasheet page62
*******************************************************************************/
int8_t IcmSetBias()
{
    icm42605_write_reg(ICM42605_REG_BANK_SEL, 4);
	
		icm42605_write_reg(ICM42605_OFFSET_USER5, 0xE3);
		icm42605_write_reg(ICM42605_OFFSET_USER4, 0xFF);
	
		icm42605_write_reg(ICM42605_OFFSET_USER6, 0xFF);
		icm42605_write_reg(ICM42605_OFFSET_USER7, 0x0F);
		
		icm42605_write_reg(ICM42605_OFFSET_USER8, 0x14);
	
		icm42605_write_reg(ICM42605_OFFSET_USER0, 0xEA);
		icm42605_write_reg(ICM42605_OFFSET_USER1, 0x0F);		
		
		icm42605_write_reg(ICM42605_OFFSET_USER2, 0x28);
	
		icm42605_write_reg(ICM42605_OFFSET_USER3, 0xF4);
    return 0;
}
/*******************************************************************************
* 名    称： Icm42605Init
* 功    能： Icm42605 初始化寄存器配置
* 入口参数： 无
* 出口参数： 无
* 作　　者： zhaohe.
* 创建日期： 2022-01-10
* 修    改：
* 修改日期：
* 备    注：
*******************************************************************************/
int8_t Icm42605Init(void)
{
    uint8_t reg_val = 0;
    /* 读取 who am i 寄存器 */
    reg_val = icm42605_read_reg(ICM42605_WHO_AM_I);
	ICM42605DelayMs(100);
		reg_val = icm42605_read_reg(ICM42605_WHO_AM_I);
	ICM42605DelayMs(100);
		reg_val = icm42605_read_reg(ICM42605_WHO_AM_I);
	ICM42605DelayMs(100);
		reg_val = icm42605_read_reg(ICM42605_WHO_AM_I);
    icm42605_write_reg(ICM42605_REG_BANK_SEL, 0);    //设置bank 0区域寄存器
		icm42605_write_reg(ICM42605_OFFSET_USER5, 0xFF);
    icm42605_write_reg(ICM42605_REG_BANK_SEL, 0x01); //软复位传感器
    ICM42605DelayMs(100);
    if (reg_val == ICM42605_ID)
    {
        icm42605_write_reg(ICM42605_REG_BANK_SEL, 1);    //设置bank 1区域寄存器
        icm42605_write_reg(ICM42605_INTF_CONFIG4, 0x02); //设置为4线SPI通信
				
				//IcmSetBias(); 
			
        icm42605_write_reg(ICM42605_REG_BANK_SEL, 0);   //设置bank 0区域寄存器
        icm42605_write_reg(ICM42605_FIFO_CONFIG, 0x40); //Stream-to-FIFO Mode(page61)
				
        reg_val = icm42605_read_reg(ICM42605_INT_SOURCE0);
        icm42605_write_reg(ICM42605_INT_SOURCE0, 0x00);
        icm42605_write_reg(ICM42605_FIFO_CONFIG2, 0x00); // watermark
        icm42605_write_reg(ICM42605_FIFO_CONFIG3, 0x02); // watermark
        icm42605_write_reg(ICM42605_INT_SOURCE0, reg_val);
        icm42605_write_reg(ICM42605_FIFO_CONFIG1, 0x63); // Enable the accel and gyro to the FIFO

        icm42605_write_reg(ICM42605_REG_BANK_SEL, 0x00);
        icm42605_write_reg(ICM42605_INT_CONFIG, 0x36);

        icm42605_write_reg(ICM42605_REG_BANK_SEL, 0x00);
        reg_val = icm42605_read_reg(ICM42605_INT_SOURCE0);
        reg_val |= (1 << 2); //FIFO_THS_INT1_ENABLE
        icm42605_write_reg(ICM42605_INT_SOURCE0, reg_val);

        Icm42605GetAres(AFS_8G);
        icm42605_write_reg(ICM42605_REG_BANK_SEL, 0x00);
        reg_val = icm42605_read_reg(ICM42605_ACCEL_CONFIG0); //page74
        reg_val |= (AFS_8G << 5);                            //量程 ±8g
        reg_val |= (AODR_1000Hz);                             //输出速率 50HZ
        icm42605_write_reg(ICM42605_ACCEL_CONFIG0, reg_val);
				
				
        Icm42605GetGres(GFS_1000DPS);
        icm42605_write_reg(ICM42605_REG_BANK_SEL, 0x00);
        reg_val = icm42605_read_reg(ICM42605_GYRO_CONFIG0); //page73
        reg_val |= (GFS_1000DPS << 5);                      //量程 ±1000dps
        reg_val |= (GODR_1000Hz);                            //输出速率 50HZ
        icm42605_write_reg(ICM42605_GYRO_CONFIG0, reg_val);

        icm42605_write_reg(ICM42605_REG_BANK_SEL, 0x00);
        reg_val = icm42605_read_reg(ICM42605_PWR_MGMT0); //读取PWR—MGMT0当前寄存器的值(page72)
        reg_val &= ~(1 << 5);                            //使能温度测量
        reg_val |= ((3) << 2);                           //设置GYRO_MODE  0:关闭 1:待机 2:预留 3:低噪声
        reg_val |= (3);                                  //设置ACCEL_MODE 0:关闭 1:关闭 2:低功耗 3:低噪声
        icm42605_write_reg(ICM42605_PWR_MGMT0, reg_val);
        ICM42605DelayMs(1); //操作完PWR—MGMT0寄存器后 200us内不能有任何读写寄存器的操作

        return 0;
    }
    return -1;
}

/*******************************************************************************
* 名    称： IcmGetTemperature
* 功    能： 读取Icm42605 内部传感器温度
* 入口参数： 无
* 出口参数： 无
* 作　　者： Roger-WY.
* 创建日期： 2021-05-21
* 修    改：
* 修改日期：
* 备    注： datasheet page62
*******************************************************************************/
int8_t IcmGetTemperature(int16_t *pTemp)
{
    icm42605_read_regs(ICM42605_TEMP_DATA1, 2);

    *pTemp = (int16_t)(((int16_t)((dataBuf[0] << 8) | dataBuf[1])) / 132.48 + 25);
    return 0;
}

int8_t IcmGetOffSet(uint8_t *set_off)
{
    icm42605_read_regs(ICM42605_OFFSET_USER5, 1);

    *set_off = dataBuf[0];
    return 0;
}
/*******************************************************************************
* 名    称： IcmGetAccelerometer
* 功    能： 读取Icm42605 加速度的值
* 入口参数： 三轴加速度的值
* 出口参数： 无
* 作　　者： Roger-WY.
* 创建日期： 2021-05-21
* 修    改：
* 修改日期：
* 备    注： datasheet page62
*******************************************************************************/
int8_t IcmGetAccelerometer(icm42605RawData_t *accData)
{
    icm42605_read_regs(ICM42605_ACCEL_DATA_X1, 6);

    accData->x = ((uint16_t)dataBuf[0] << 8) | dataBuf[1];
    accData->y = ((uint16_t)dataBuf[2] << 8) | dataBuf[3];
    accData->z = ((uint16_t)dataBuf[4] << 8) | dataBuf[5];

    accData->x = (int16_t)(accData->x * accSensitivity);
    accData->y = (int16_t)(accData->y * accSensitivity);
    accData->z = (int16_t)(accData->z * accSensitivity);

    return 0;
}

/*******************************************************************************
* 名    称： IcmGetGyroscope
* 功    能： 读取Icm42605 陀螺仪的值
* 入口参数： 三轴陀螺仪的值
* 出口参数： 无
* 作　　者： Roger-WY.
* 创建日期： 2021-05-21
* 修    改：
* 修改日期：
* 备    注： datasheet page63
*******************************************************************************/
int8_t IcmGetGyroscope(icm42605RawData_t *GyroData)
{
    icm42605_read_regs(ICM42605_GYRO_DATA_X1, 6);

    GyroData->x = ((uint16_t)dataBuf[0] << 8) | dataBuf[1];
    GyroData->y = ((uint16_t)dataBuf[2] << 8) | dataBuf[3];
    GyroData->z = ((uint16_t)dataBuf[4] << 8) | dataBuf[5];

    GyroData->x = (int16_t)(GyroData->x * gyroSensitivity);
    GyroData->y = (int16_t)(GyroData->y * gyroSensitivity);
    GyroData->z = (int16_t)(GyroData->z * gyroSensitivity);
    return 0;
}

/*******************************************************************************
* 名    称： IcmGetRawData
* 功    能： 读取Icm42605加速度陀螺仪数据
* 入口参数： 六轴
* 出口参数： 无
* 作　　者： Roger-WY.
* 创建日期： 2021-05-21
* 修    改：
* 修改日期：
* 备    注： datasheet page62,63
*******************************************************************************/
int8_t IcmGetRawData(icm42605RawData_t *accData, icm42605RawData_t *GyroData)
{
    icm42605_read_regs(ICM42605_ACCEL_DATA_X1, 12);

    accData->x = ((uint16_t)dataBuf[0] << 8) | dataBuf[1];
    accData->y = ((uint16_t)dataBuf[2] << 8) | dataBuf[3];
    accData->z = ((uint16_t)dataBuf[4] << 8) | dataBuf[5];
    GyroData->x = ((uint16_t)dataBuf[6] << 8) | dataBuf[7];
    GyroData->y = ((uint16_t)dataBuf[8] << 8) | dataBuf[9];
    GyroData->z = ((uint16_t)dataBuf[10] << 8) | dataBuf[11];

    accData->x = (int16_t)(accData->x * accSensitivity);
    accData->y = (int16_t)(accData->y * accSensitivity);
    accData->z = (int16_t)(accData->z * accSensitivity);

    GyroData->x = (int16_t)(GyroData->x * gyroSensitivity);
    GyroData->y = (int16_t)(GyroData->y * gyroSensitivity);
    GyroData->z = (int16_t)(GyroData->z * gyroSensitivity);

    return 0;
}



/*******************************************************************************
* 名    称： IcmGetData
* 功    能： 读取Icm42605加速度陀螺仪数据
* 入口参数： 六轴
* 出口参数： 无
* 作　　者： zhaohe
* 创建日期： 2021-05-21
* 修    改：
* 修改日期：
* 备    注： datasheet page62,63
*******************************************************************************/
int8_t IcmGetData(ICM42605Data *icm42605data)
{
	icm42605_read_regs(ICM42605_ACCEL_DATA_X1, 12);
	icm42605data->raw_acc.x = ((uint16_t)dataBuf[0] << 8) | dataBuf[1];
	icm42605data->raw_acc.y = ((uint16_t)dataBuf[2] << 8) | dataBuf[3];
	icm42605data->raw_acc.z = ((uint16_t)dataBuf[4] << 8) | dataBuf[5];
	icm42605data->raw_gyro.x = ((uint16_t)dataBuf[6] << 8) | dataBuf[7];
	icm42605data->raw_gyro.y = ((uint16_t)dataBuf[8] << 8) | dataBuf[9];
	icm42605data->raw_gyro.z = ((uint16_t)dataBuf[10] << 8) | dataBuf[11];
	icm42605data->acc.x = (float)(int16_t)(icm42605data->raw_acc.x) * accSensitivity;
	icm42605data->acc.y = (float)(int16_t)(icm42605data->raw_acc.y) * accSensitivity;
	icm42605data->acc.z = (float)(int16_t)(icm42605data->raw_acc.z) * accSensitivity;
	icm42605data->gyro.x = (float)(int16_t)(icm42605data->raw_gyro.x) * gyroSensitivity;
	icm42605data->gyro.y = (float)(int16_t)(icm42605data->raw_gyro.y) * gyroSensitivity;
	icm42605data->gyro.z = (float)(int16_t)(icm42605data->raw_gyro.z) * gyroSensitivity;
	return 0;
}

int8_t IcmWhoAmI()
{
	int8_t reg_val = icm42605_read_reg(ICM42605_WHO_AM_I);
	return reg_val;
}
