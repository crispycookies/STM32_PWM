/*
 * PWM_Driver.c
 *
 *  Created on: 12.10.2018
 *      Author: tobi
 */

#include "PWM.h"

bool PWM_Base::write() {
    if (!is_init) {
        return false;
    }

    //CLOCK
    __HAL_TIM_ENABLE(&timer_tdef);
    //CONFIG
    HAL_TIM_PWM_Init(&timer_tdef);
    HAL_TIM_PWM_ConfigChannel(&timer_tdef, &pwm_tdef, channel);
    HAL_TIM_PWM_Start(&timer_tdef, channel);

    return false;
}
bool PWM_Base::disable() {
    gp_pair.init = false;
    lut[i].init = false;
    HAL_GPIO_DeInit(gp_pair.gpio.bank, gp_pair.gpio.pin);
    HAL_TIM_PWM_DeInit(&timer_tdef);
    return false;
}
bool PWM_Base::set_pin(const GPIO_Pair gp) {

    i = 0;

    if (lut_len == 0 || lut == nullptr) {
        return false;
    }
    for (; i < lut_len; ++i) {
        if (lut[i].gpio.bank == gp.bank && lut[i].gpio.pin == gp.pin
            && !lut[i].init) {
            gp_pair.gpio.bank = gp.bank;
            gp_pair.gpio.pin = gp.pin;
            gp_pair.af_mode = lut[i].af_mode;
            gp_pair.init = true;
            lut[i].init = true;
            return true;
        }
    }
    if (i == lut_len) {
        i = 0;
        return false;
    }
    if (!init_gpio()) {
        i = 0;
        return false;
    }

    return true;
}

bool PWM_Base::set_pulse_len(const pulse_t pulse_len) {

    if (pulse_len == 0) {
        return false;
    }

    pwm_tdef.OCFastMode = TIM_OCFAST_ENABLE;
    pwm_tdef.OCIdleState = TIM_OCIDLESTATE_RESET;
    pwm_tdef.OCMode = TIM_OCMODE_PWM1;
    pwm_tdef.Pulse = pulse_len;
    pwm_tdef.OCNIdleState = TIM_OCNIDLESTATE_SET;
    pwm_tdef.OCNPolarity = TIM_OCPOLARITY_LOW;
    pwm_tdef.OCPolarity = TIM_OCPOLARITY_HIGH;

    return true;
}
bool PWM_Base::set_period_len(const pulse_t period_len) {
    timbase_tdef.Period = period_len;

    timbase_tdef.CounterMode = TIM_COUNTERMODE_UP;
    timbase_tdef.RepetitionCounter = 0;
    timbase_tdef.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    return true;
}
bool PWM_Base::set_prescaler(const pulse_t presc_len) {
    timbase_tdef.Prescaler = presc_len;
    return true;
}
bool PWM_Base::init_gpio() {
    init_gpio_clk();
    pwm_gpio_tdef.Mode = GPIO_MODE_AF_PP;
    pwm_gpio_tdef.Pin = gp_pair.gpio.pin;
    pwm_gpio_tdef.Pull = GPIO_NOPULL;
    pwm_gpio_tdef.Speed = GPIO_SPEED_HIGH;
    pwm_gpio_tdef.Alternate = gp_pair.af_mode;
    HAL_GPIO_Init(gp_pair.gpio.bank, &pwm_gpio_tdef);
    return true;
}
bool PWM_Base::init_gpio_clk() {
    if (gp_pair.gpio.bank == GPIOA) {
        __HAL_RCC_GPIOA_CLK_ENABLE()
                ;
    } else if (gp_pair.gpio.bank == GPIOB) {
        __HAL_RCC_GPIOB_CLK_ENABLE()
                ;
    } else if (gp_pair.gpio.bank == GPIOC) {
        __HAL_RCC_GPIOC_CLK_ENABLE()
                ;
    } else if (gp_pair.gpio.bank == GPIOD) {
        __HAL_RCC_GPIOD_CLK_ENABLE()
                ;
    } else if (gp_pair.gpio.bank == GPIOE) {
        __HAL_RCC_GPIOE_CLK_ENABLE()
                ;
    } else if (gp_pair.gpio.bank == GPIOF) {
        __HAL_RCC_GPIOF_CLK_ENABLE()
                ;
    } else if (gp_pair.gpio.bank == GPIOG) {
        __HAL_RCC_GPIOG_CLK_ENABLE()
                ;
    } else if (gp_pair.gpio.bank == GPIOH) {
        __HAL_RCC_GPIOH_CLK_ENABLE()
                ;
    } else {
        return false;
    }

    return true;
}
bool PWM_Base::init_tim() {
    HAL_TIM_PWM_Init(&timer_tdef);
    return false;
}
bool PWM_Base::init_tim_pwm() {
    return false;
}
bool PWM_Base::set_lut(PWM_GPIO_TIM_STRUCT * _lut, uint16_t _lut_len) {
    if (_lut != nullptr && _lut_len != 0) {
        lut = _lut;
        lut_len = _lut_len;
        return true;
    }
    return false;
}
bool PWM_Base::set_timer(TIM_TypeDef * _timer, uint32_t _channel) {
    if (_timer != nullptr) {
        timer = _timer;
        channel = _channel;
        return true;
    }
    return false;
}
bool PWM_Base::init() {
    if (init_gpio() && init_tim() && init_tim_pwm()) {
        is_init = true;
        return true;
    }
    return false;
}
