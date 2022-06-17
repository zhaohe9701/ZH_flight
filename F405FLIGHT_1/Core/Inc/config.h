#ifndef __CONFIG_H__
#define __CONFIG_H__

#define REMOTE_UART                 (huart2)
#define UPPER_COMPUTER_UART         (huart3)
#define MAIN_LOOP_TIMER             (htim7)
#define SENSOR_LOOP_TIMER           (htim6)
#define IMU_SPI                     (hspi1)

#define REMOTE_UART_MARK            USART2
#define UPPER_COMPUTER_UART_MARK    USART3

#define GYRO_EXTI_IRQ               EXTI4_IRQn

#define MAIN_LOOP_TIMER_MARK        TIM7
#define SENSOR_LOOP_TIMER_MARK      TIM6
#define MOTOR_1_TIM                 (htim3)
#define MOTOR_1_TIM_CHANNEL         TIM_CHANNEL_3

#define MOTOR_1_TIM_CHANNEL_END     HAL_TIM_ACTIVE_CHANNEL_3

#define MOTOR_2_TIM                 (htim3)
#define MOTOR_2_TIM_CHANNEL         TIM_CHANNEL_4
#define MOTOR_2_TIM_CHANNEL_END     HAL_TIM_ACTIVE_CHANNEL_4

#define MOTOR_3_TIM                 (htim8)
#define MOTOR_3_TIM_CHANNEL         TIM_CHANNEL_4
#define MOTOR_3_TIM_CHANNEL_END     HAL_TIM_ACTIVE_CHANNEL_4

#define MOTOR_4_TIM                 (htim8)
#define MOTOR_4_TIM_CHANNEL         TIM_CHANNEL_3
#define MOTOR_4_TIM_CHANNEL_END     HAL_TIM_ACTIVE_CHANNEL_3

#define MAIN_CYCLE_FREQ             500.0f
#define SENSOR_CYCLE_FREQ           1000.0f

#define GYRO_LPF_CUTOFF_FREQ        100
#define ACCEL_LPF_CUTOFF_FREQ       50

#endif