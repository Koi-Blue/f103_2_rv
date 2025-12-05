// #ifndef __MOTOR_CONTROL_H
// #define __MOTOR_CONTROL_H
//
// #ifdef __cplusplus
// extern "C" {
// #endif
//
// #include "../startup/stm32f10x.h"
//
// // 电机方向枚举（C/C++共用）
// typedef enum {
//     MOTOR_FORWARD = 0,  // 正转
//     MOTOR_BACKWARD = 1, // 反转
//     MOTOR_STOP = 2      // 停止
// } MotorDirection;
//
// // 电机句柄（C接口句柄，实际指向C++对象）
// typedef void* Motor_Handle;
//
// /**
//  * @brief  创建电机对象并初始化
//  * @param  TIMx: PWM定时器（如TIM1/TIM2）
//  * @param  PWM_Channel: PWM通道（如TIM_Channel_1）
//  * @param  Dir_GPIOx: 方向控制GPIO组
//  * @param  Dir_Pin1: 方向引脚1
//  * @param  Dir_Pin2: 方向引脚2
//  * @param  PWM_Period: PWM周期（决定频率）
//  * @retval 电机句柄（NULL表示失败）
//  */
// Motor_Handle Motor_Create(TIM_TypeDef* TIMx, uint16_t PWM_Channel,
//                           GPIO_TypeDef* Dir_GPIOx, uint16_t Dir_Pin1, uint16_t Dir_Pin2,
//                           uint16_t PWM_Period);
//
// /**
//  * @brief  设置电机方向
//  * @param  handle: 电机句柄
//  * @param  dir: 电机方向
//  * @retval None
//  */
// void Motor_SetDirection(Motor_Handle handle, MotorDirection dir);
//
// /**
//  * @brief  设置电机速度（占空比）
//  * @param  handle: 电机句柄
//  * @param  duty: 占空比（0-100）
//  * @retval None
//  */
// void Motor_SetSpeed(Motor_Handle handle, uint8_t duty);
//
// /**
//  * @brief  电机停止
//  * @param  handle: 电机句柄
//  * @retval None
//  */
// void Motor_Stop(Motor_Handle handle);
//
// /**
//  * @brief  销毁电机对象
//  * @param  handle: 电机句柄
//  * @retval None
//  */
// void Motor_Destroy(Motor_Handle handle);
//
// #ifdef __cplusplus
// }
// #endif
//
// // C++类定义（仅C++可见）
// #ifdef __cplusplus
// class MotorController {
// private:
//     TIM_TypeDef* m_TIMx;          // PWM定时器
//     uint16_t m_PWMChannel;        // PWM通道
//     GPIO_TypeDef* m_DirGPIOx;     // 方向GPIO组
//     uint16_t m_DirPin1;           // 方向引脚1
//     uint16_t m_DirPin2;           // 方向引脚2
//     uint16_t m_PWMPeriod;         // PWM周期
//
//     // 初始化硬件
//     void initHardware();
//
// public:
//     // 构造函数
//     MotorController(TIM_TypeDef* TIMx, uint16_t PWMChannel,
//                     GPIO_TypeDef* DirGPIOx, uint16_t DirPin1, uint16_t DirPin2,
//                     uint16_t PWMPeriod);
//
//     // 析构函数
//     ~MotorController();
//
//     // 设置方向
//     void setDirection(MotorDirection dir);
//
//     // 设置速度
//     void setSpeed(uint8_t duty);
//
//     // 停止电机
//     void stop();
// };
// #endif
//
// #endif /* __MOTOR_CONTROL_H */