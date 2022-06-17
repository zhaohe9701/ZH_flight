#include "dshot.h"
#include "config.h"
//定时器 4分频72/4=18mhz ;分频不固定，可自行调整
//pwm波周期 1.67us ；对应的pwm分辨率 1.67us /（1/18）= 30；
// 0.625us/(1/18) = 11; 0的占空比11/30
// 1.250us/(1/18) = 22; 1的占空比22/30


//uint16_t ESC_CMD[ESC_CMD_BUF_LEN] = {0};
uint16_t esc_cmd[ESC_CMD_BUF_LEN] = {0};
extern TIM_HandleTypeDef MOTOR_1_TIM;
extern TIM_HandleTypeDef MOTOR_3_TIM;


/*************************************************************
** Function name:       prepareDshotPacket
** Descriptions:        CRC校验以及要不要回传信息
** Input parameters:    value: 油门大小 注意取值范围, requestTelemetry 是否请求回传数据
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
static uint16_t prepareDshotPacket(const uint16_t value, bool requestTelemetry)
{
	// 油门大小为11位  所以这里先左移一位 添加上请求回传标志共12位
	uint16_t packet = (value << 1) | (requestTelemetry ? 1 : 0);

	// 将12位数据分为3组 每组4位, 进行异或
	// compute checksum
	uint16_t csum = 0;
	uint16_t csum_data = packet;
	for (int i = 0; i < 3; i++)
	{
		csum ^= csum_data; // xor data by nibbles
		csum_data >>= 4;
	}
	//取最后四位 其他的不要
	csum &= 0xf;
	// append checksum 将CRC添加到后四位
	packet = (packet << 4) | csum;
	return packet;
}

/*************************************************************
** Function name:       pwmWriteDigital
** Descriptions:        根据输入 填充esc_cmd,填充的数值代表每一位的高低电平
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
static void pwmWriteDigital(uint16_t *esc_cmd, uint16_t value)
{
	value = ((value > 2047) ? 2047 : value);
	value = prepareDshotPacket(value, 0);
	esc_cmd[0] = (value & 0x8000) ? ESC_BIT_1 : ESC_BIT_0;
	esc_cmd[1] = (value & 0x4000) ? ESC_BIT_1 : ESC_BIT_0;
	esc_cmd[2] = (value & 0x2000) ? ESC_BIT_1 : ESC_BIT_0;
	esc_cmd[3] = (value & 0x1000) ? ESC_BIT_1 : ESC_BIT_0;
	esc_cmd[4] = (value & 0x0800) ? ESC_BIT_1 : ESC_BIT_0;
	esc_cmd[5] = (value & 0x0400) ? ESC_BIT_1 : ESC_BIT_0;
	esc_cmd[6] = (value & 0x0200) ? ESC_BIT_1 : ESC_BIT_0;
	esc_cmd[7] = (value & 0x0100) ? ESC_BIT_1 : ESC_BIT_0;
	esc_cmd[8] = (value & 0x0080) ? ESC_BIT_1 : ESC_BIT_0;
	esc_cmd[9] = (value & 0x0040) ? ESC_BIT_1 : ESC_BIT_0;
	esc_cmd[10] = (value & 0x0020) ? ESC_BIT_1 : ESC_BIT_0;
	esc_cmd[11] = (value & 0x0010) ? ESC_BIT_1 : ESC_BIT_0;
	esc_cmd[12] = (value & 0x8) ? ESC_BIT_1 : ESC_BIT_0;
	esc_cmd[13] = (value & 0x4) ? ESC_BIT_1 : ESC_BIT_0;
	esc_cmd[14] = (value & 0x2) ? ESC_BIT_1 : ESC_BIT_0;
	esc_cmd[15] = (value & 0x1) ? ESC_BIT_1 : ESC_BIT_0;
}

/*************************************************************
** Function name:       HAL_TIM_PWM_PulseFinishedCallback
** Descriptions:        每次DMA发送完后会进这个回调函数，可以做标记。要记得关闭DMA
** Input parameters:    None
** Output parameters:   None
** Returned value:      None
** Remarks:             None
*************************************************************/
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &MOTOR_1_TIM && htim->Channel == MOTOR_1_TIM_CHANNEL_END)
	{
		HAL_TIM_PWM_Stop_DMA(&MOTOR_1_TIM, MOTOR_1_TIM_CHANNEL);
		//usb_printf("M1 end\r\n");
	}
	else if(htim == &MOTOR_2_TIM && htim->Channel == MOTOR_2_TIM_CHANNEL_END)
	{
		HAL_TIM_PWM_Stop_DMA(&MOTOR_2_TIM, MOTOR_2_TIM_CHANNEL);
		//usb_printf("M2 end\r\n");
	}
	else if(htim == &MOTOR_3_TIM && htim->Channel == MOTOR_3_TIM_CHANNEL_END)
	{
		HAL_TIM_PWM_Stop_DMA(&MOTOR_3_TIM, MOTOR_3_TIM_CHANNEL);
		//usb_printf("M3 end\r\n");
	}
	else if(htim == &MOTOR_4_TIM && htim->Channel == MOTOR_4_TIM_CHANNEL_END)
	{
		HAL_TIM_PWM_Stop_DMA(&MOTOR_4_TIM, MOTOR_4_TIM_CHANNEL);
		//usb_printf("M4 end\r\n");
	}
	else;
	
}

void SetMotorValue(uint16_t *motor)
{
	static uint16_t motor1_cmd[ESC_CMD_BUF_LEN] = {0};
	static uint16_t motor2_cmd[ESC_CMD_BUF_LEN] = {0};
	static uint16_t motor3_cmd[ESC_CMD_BUF_LEN] = {0};
	static uint16_t motor4_cmd[ESC_CMD_BUF_LEN] = {0};
	pwmWriteDigital(motor1_cmd, motor[0]);
	pwmWriteDigital(motor2_cmd, motor[1]);
	pwmWriteDigital(motor3_cmd, motor[2]);
	pwmWriteDigital(motor4_cmd, motor[3]);
	HAL_TIM_PWM_Start_DMA(&MOTOR_1_TIM, MOTOR_1_TIM_CHANNEL, (uint32_t *)motor1_cmd, ESC_CMD_BUF_LEN);
	HAL_TIM_PWM_Start_DMA(&MOTOR_2_TIM, MOTOR_2_TIM_CHANNEL, (uint32_t *)motor2_cmd, ESC_CMD_BUF_LEN);
	HAL_TIM_PWM_Start_DMA(&MOTOR_3_TIM, MOTOR_3_TIM_CHANNEL, (uint32_t *)motor3_cmd, ESC_CMD_BUF_LEN);
	HAL_TIM_PWM_Start_DMA(&MOTOR_4_TIM, MOTOR_4_TIM_CHANNEL, (uint32_t *)motor4_cmd, ESC_CMD_BUF_LEN);
}