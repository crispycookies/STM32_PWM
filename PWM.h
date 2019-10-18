/*
 * PWM_Driver_c.h
 *
 *  Created on: 12.10.2018
 *      Author: tobi
 */

#ifndef GROUPS_GROUP_BRICKS_PWM_BACKEND_PWM_BASE_COMMON_BASE_C_PWM_DRIVER_H_
#define GROUPS_GROUP_BRICKS_PWM_BACKEND_PWM_BASE_COMMON_BASE_C_PWM_DRIVER_H_

#include <stdbool.h>
#include "stm32f4xx.h"
#include "../../../Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h"
#include "../../../Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_tim.h"
#include "../../../Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_gpio.h"

class PWM_Base {
public:
    typedef uint32_t pulse_t;
    typedef uint32_t GPIO_PinType;
    struct GPIO_Pair {
        GPIO_PinType pin = 0;
        GPIO_TypeDef * bank = nullptr;
    };
    struct PWM_GPIO_TIM_STRUCT {
        GPIO_Pair gpio;
        uint32_t af_mode = 0;
        bool init = false;
    };
    PWM_Base() = default;
    virtual ~PWM_Base() = default;
    bool init();
    bool write();
    bool disable();
    bool set_pin(const GPIO_Pair gp);
    bool set_pulse_len(const pulse_t pulse_len);
    bool set_period_len(const pulse_t period_len);
    bool set_lut(PWM_GPIO_TIM_STRUCT * _lut, uint16_t _lut_len);
    bool set_timer(TIM_TypeDef * _timer, uint32_t _channel);
    bool set_prescaler(const pulse_t presc_len);
protected:
    PWM_GPIO_TIM_STRUCT * lut = nullptr;
    TIM_TypeDef * timer = nullptr;
    uint16_t lut_len = 0;
    uint32_t channel = 0;

    PWM_GPIO_TIM_STRUCT gp_pair;

    TIM_Base_InitTypeDef timbase_tdef;
    TIM_HandleTypeDef timer_tdef;
    TIM_OC_InitTypeDef pwm_tdef;

    GPIO_InitTypeDef pwm_gpio_tdef;
    bool is_init = false;

    int i = 0;

    bool init_gpio();
    bool init_gpio_clk();
    bool init_tim();
    bool init_tim_pwm();
};

#endif /* GROUPS_GROUP_BRICKS_PWM_BACKEND_PWM_BASE_COMMON_BASE_C_PWM_DRIVER_H_ */
