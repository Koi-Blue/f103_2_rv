// #include "motor_control.h"
//
// // C++类成员实现
// MotorController::MotorController(TIM_TypeDef* TIMx, uint16_t PWMChannel,
//                                  GPIO_TypeDef* DirGPIOx, uint16_t DirPin1, uint16_t DirPin2,
//                                  uint16_t PWMPeriod) {
//     m_TIMx = TIMx;
//     m_PWMChannel = PWMChannel;
//     m_DirGPIOx = DirGPIOx;
//     m_DirPin1 = DirPin1;
//     m_DirPin2 = DirPin2;
//     m_PWMPeriod = PWMPeriod;
//
//     initHardware(); // 初始化硬件
// }
//
// MotorController::~MotorController() {
//     stop(); // 销毁前停止电机
// }
//
// void MotorController::initHardware() {
//     GPIO_InitTypeDef GPIO_InitStruct;
//     TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
//     TIM_OCInitTypeDef TIM_OCInitStruct;
//
//     // 1. 使能时钟
//     // PWM定时器时钟
//     if (m_TIMx == TIM1) RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
//     else if (m_TIMx == TIM2) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//     else if (m_TIMx == TIM3) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//     else if (m_TIMx == TIM4) RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
//
//     // 方向引脚GPIO时钟
//     if (m_DirGPIOx == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//     else if (m_DirGPIOx == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//     else if (m_DirGPIOx == GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//
//     // 2. 配置方向引脚（推挽输出）
//     GPIO_InitStruct.GPIO_Pin = m_DirPin1 | m_DirPin2;
//     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
//     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//     GPIO_Init(m_DirGPIOx, &GPIO_InitStruct);
//
//     // 初始停止
//     GPIO_SetBits(m_DirGPIOx, m_DirPin1 | m_DirPin2);
//
//     // 3. 配置PWM定时器基础参数
//     TIM_TimeBaseStruct.TIM_Prescaler = 71; // 72MHz / 72 = 1MHz计数频率
//     TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
//     TIM_TimeBaseStruct.TIM_Period = m_PWMPeriod - 1;
//     TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
//     TIM_TimeBaseInit(m_TIMx, &TIM_TimeBaseStruct);
//
//     // 4. 配置PWM输出通道
//     TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
//     TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
//     TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
//
//     switch (m_PWMChannel) {
//         case TIM_Channel_1:
//             TIM_OC1Init(m_TIMx, &TIM_OCInitStruct);
//             TIM_OC1PreloadConfig(m_TIMx, TIM_OCPreload_Enable);
//             break;
//         case TIM_Channel_2:
//             TIM_OC2Init(m_TIMx, &TIM_OCInitStruct);
//             TIM_OC2PreloadConfig(m_TIMx, TIM_OCPreload_Enable);
//             break;
//         case TIM_Channel_3:
//             TIM_OC3Init(m_TIMx, &TIM_OCInitStruct);
//             TIM_OC3PreloadConfig(m_TIMx, TIM_OCPreload_Enable);
//             break;
//         case TIM_Channel_4:
//             TIM_OC4Init(m_TIMx, &TIM_OCInitStruct);
//             TIM_OC4PreloadConfig(m_TIMx, TIM_OCPreload_Enable);
//             break;
//     }
//
//     TIM_ARRPreloadConfig(m_TIMx, ENABLE);
//     TIM_Cmd(m_TIMx, ENABLE);
//
//     // TIM1高级定时器需使能主输出
//     if (m_TIMx == TIM1) TIM_CtrlPWMOutputs(m_TIMx, ENABLE);
// }
//
// void MotorController::setDirection(MotorDirection dir) {
//     switch (dir) {
//         case MOTOR_FORWARD:
//             GPIO_SetBits(m_DirGPIOx, m_DirPin1);
//             GPIO_ResetBits(m_DirGPIOx, m_DirPin2);
//             break;
//         case MOTOR_BACKWARD:
//             GPIO_ResetBits(m_DirGPIOx, m_DirPin1);
//             GPIO_SetBits(m_DirGPIOx, m_DirPin2);
//             break;
//         case MOTOR_STOP:
//             GPIO_SetBits(m_DirGPIOx, m_DirPin1 | m_DirPin2);
//             break;
//     }
// }
//
// void MotorController::setSpeed(uint8_t duty) {
//     uint16_t compare = (duty * m_PWMPeriod) / 100;
//     switch (m_PWMChannel) {
//         case TIM_Channel_1: TIM_SetCompare1(m_TIMx, compare); break;
//         case TIM_Channel_2: TIM_SetCompare2(m_TIMx, compare); break;
//         case TIM_Channel_3: TIM_SetCompare3(m_TIMx, compare); break;
//         case TIM_Channel_4: TIM_SetCompare4(m_TIMx, compare); break;
//     }
// }
//
// void MotorController::stop() {
//     setDirection(MOTOR_STOP);
//     setSpeed(0);
// }
//
// // C风格接口实现（封装C++对象）
// Motor_Handle Motor_Create(TIM_TypeDef* TIMx, uint16_t PWMChannel,
//                           GPIO_TypeDef* DirGPIOx, uint16_t DirPin1, uint16_t DirPin2,
//                           uint16_t PWMPeriod) {
//     // 创建C++对象并返回句柄
//     return new MotorController(TIMx, PWMChannel, DirGPIOx, DirPin1, DirPin2, PWMPeriod);
// }
//
// void Motor_SetDirection(Motor_Handle handle, MotorDirection dir) {
//     if (handle) {
//         static_cast<MotorController*>(handle)->setDirection(dir);
//     }
// }
//
// void Motor_SetSpeed(Motor_Handle handle, uint8_t duty) {
//     if (handle) {
//         static_cast<MotorController*>(handle)->setSpeed(duty);
//     }
// }
//
// void Motor_Stop(Motor_Handle handle) {
//     if (handle) {
//         static_cast<MotorController*>(handle)->stop();
//     }
// }
//
// void Motor_Destroy(Motor_Handle handle) {
//     if (handle) {
//         delete static_cast<MotorController*>(handle);
//     }
// }