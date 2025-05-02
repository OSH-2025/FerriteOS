/* ----------------------------------------------------------------------------
* Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
* Description: Gpio Register Control Headfile
* Author: Huawei LiteOS Team
* Create: 2021-10-24
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
* 1.Redistributions of source code must retain the above copyright notice, this list of
* conditions and the following disclaimer.
* 2.Redistributions in binary form must reproduce the above copyright notice, this list
* of conditions and the following disclaimer in the documentation and/or other materials
* provided with the distribution.
* 3.Neither the name of the copyright holder nor the names of its contributors may be used
* to endorse or promote products derived from this software without specific prior written
* permission.
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*--------------------------------------------------------------------------- */
#ifndef __BCM2836_GPIO_H__
#define __BCM2836_GPIO_H__
#include "gpio_def.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define gpio_func_select(pin, type) \
    do { \
        UINT8 regIdx = pin / 10; \
        UINT8 regShift = pin % 10; \
        volatile UINT32 * gpio_func_sel= (volatile UINT32 *)(GPIO_FUNC_SEL_0 + (0x04 * regIdx)); \
        *gpio_func_sel = \
            ((*gpio_func_sel) & ~(GPIO_FUNC_SEL_MASK << (regShift * GPIO_FUNC_SEL_BITS))) \
               | (type << (regShift*GPIO_FUNC_SEL_BITS)); \
    } while(0);

#define gpio_output_set(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_output_set = (volatile UINT32 *)(GPIO_OUTPUT_SET_0 + (0x04 * regIdx)); \
        *gpio_output_set = ((*gpio_output_set) & ~(0x01 << regShift)) | (0x01 << regShift); \
    } while(0);

/*
The output clear registers) are used to clear a GPIO pin. The CLR{n} field defines 
the respective GPIO pin to clear, writing a “0” to the field has no effect. If the GPIO 
pin is being used as in input (by default) then the value in the CLR{n} field is 
ignored. However, if the pin is subsequently defined as an output then the bit will 
be set according to the last set/clear operation. Separating the set and clear 
functions removes the need for read-modify-write operations
*/
#define gpio_output_clr(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_output_clr = \
            (volatile UINT32 *)(GPIO_OUTPUT_CLR_0 + (0x04 * regIdx)); \
        *gpio_output_clr = \
            ((*gpio_output_clr) & ~(0x01 << regShift)) | (0x01 << regShift); \
    } while(0);

/*
The pin level registers return the actual value of the pin. The LEV{n} field gives the 
value of the respective GPIO pin
*/
#define gpio_pin_level(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_pin_level = \
            (volatile UINT32 *)(GPIO_LEVEL_0 + (0x04 * regIdx)); \
        UINT32 __val = ((*gpio_pin_level) & (0x01 << regShift)); \
        __val; \
    } while(0);

/*
 The event detect status registers are used to record level and edge events on the 
GPIO pins.
The relevant bit in the event detect status registers is set whenever:
    1) an edge is detected that matches the type of edge programmed in the 
    rising/falling edge detect enable registers, or 
    2) a level is detected that matches the type of level programmed in the 
    high/low level detect enable registers.
The bit is cleared by writing a “1” to the relevant bit. 

The interrupt controller can be programmed to interrupt the processor when any of 
the status bits are set. The GPIO peripheral has three dedicated interrupt lines. 
Each GPIO bank can generate an independent interrupt. The third line generates a 
single interrupt whenever any bit is set. 
*/
#define gpio_pin_event_detect_status(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_pin_event_status = \
            (volatile UINT32 *)(GPIO_EVENT_DETECT_STATUS_0 + (0x04 * regIdx)); \
        UINT32 __val = ((*gpio_pin_event_status)&(0x01 << regShift)); \
        __val; \
    } while(0);

#define gpio_pin_event_detect_status_clr(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_pin_event_status = \
            (volatile UINT32 *)(GPIO_EVENT_DETECT_STATUS_0 + (0x04 * regIdx)); \
        *gpio_pin_event_status = ((*gpio_pin_event_status) | (0x01 << regShift)); \
    } while(0);

/*
The rising edge detect enable registers define the pins for which a rising edge 
transition sets a bit in the event detect status registers (GPEDSn). When the 
relevant bits are set in both the GPRENn and GPFENn registers, any transition (1 
to 0 and 0 to 1) will set a bit in the GPEDSn registers. The GPRENn registers use 
synchronous edge detection. This means the input signal is sampled using the 
system clock and then it is looking for a “011” pattern on the sampled signal. This 
has the effect of suppressing glitches. 
*/
#define gpio_rising_edge_detect_enable(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_rising_detect_reg = \
            (volatile UINT32 *)(GPIO_RISING_EDGE_DETECT_EN_0 + (0x04 * regIdx)); \
        *gpio_rising_detect_reg = ((*gpio_rising_detect_reg) | (0x01 << regShift)); \
    } while(0);

#define gpio_rising_edge_detect_disable(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_rising_detect_reg = \
            (volatile UINT32 *)(GPIO_RISING_EDGE_DETECT_EN_0 + (0x04 * regIdx)); \
        *gpio_rising_detect_reg = ((*gpio_rising_detect_reg) & ~(0x01 << regShift)); \
    } while(0);

/*
The falling edge detect enable registers define the pins for which a falling edge 
transition sets a bit in the event detect status registers (GPEDSn). When the relevant 
bits are set in both the GPRENn and GPFENn registers, any transition (1 to 0 and 0 
to 1) will set a bit in the GPEDSn registers. The GPFENn registers use synchronous 
edge detection. This means the input signal is sampled using the system clock and 
then it is looking for a “100” pattern on the sampled signal. This has the effect of 
suppressing glitches. 
*/
#define gpio_falling_edge_detect_enable(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_falling_detect_reg = \
            (volatile UINT32 *)(GPIO_FALLING_EDGE_DETECT_EN_0 + (0x04 * regIdx)); \
        *gpio_falling_detect_reg = ((*gpio_falling_detect_reg) | (0x01 << regShift)); \
    } while(0);

#define gpio_falling_edge_detect_disable(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_falling_detect_reg = \
            (volatile UINT32 *)(GPIO_FALLING_EDGE_DETECT_EN_0 + (0x04 * regIdx)); \
        *gpio_falling_detect_reg = ((*gpio_falling_detect_reg) & ~(0x01 << regShift)); \
    } while(0);

/*
 The high level detect enable registers define the pins for which a high level sets a bit in 
the event detect status register (GPEDSn). If the pin is still high when an attempt is 
made to clear the status bit in GPEDSn then the status bit will remain set. 
*/
#define gpio_high_detect_enable(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_high_detect_reg = \
            (volatile UINT32 *)(GPIO_HIGH_DETECT_EN_0 + (0x04 * regIdx)); \
        *gpio_high_detect_reg = ((*gpio_high_detect_reg) | (0x01 << regShift)); \
    } while(0);

#define gpio_high_detect_disable(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_high_detect_reg = \
            (volatile UINT32 *)(GPIO_HIGH_DETECT_EN_0 + (0x04 * regIdx)); \
        *gpio_high_detect_reg = ((*gpio_high_detect_reg) & ~(0x01 << regShift)); \
    } while(0);

/*
 The low level detect enable registers define the pins for which a low level sets a bit in 
the event detect status register (GPEDSn). If the pin is still low when an attempt is 
made to clear the status bit in GPEDSn then the status bit will remain set.
*/
#define gpio_low_detect_enable(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_low_detect_reg = \
            (volatile UINT32 *)(GPIO_LOW_DETECT_EN_0 + (0x04 * regIdx)); \
        *gpio_low_detect_reg = ((*gpio_low_detect_reg) | (0x01 << regShift)); \
    } while(0);

#define gpio_low_detect_disable(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_low_detect_reg = \
            (volatile UINT32 *)(GPIO_LOW_DETECT_EN_0 + (0x04 * regIdx)); \
        *gpio_low_detect_reg = ((*gpio_low_detect_reg) & ~(0x01 << regShift)); \
    } while(0);

/*
The asynchronous rising edge detect enable registers define the pins for which a 
asynchronous rising edge transition sets a bit in the event detect status registers 
(GPEDSn). 
Asynchronous means the incoming signal is not sampled by the system clock. As such 
rising edges of very short duration can be detected. 
*/
#define gpio_asyn_rising_edge_detect_enable(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_asyn_rising_detect_reg = \
            (volatile UINT32 *)(GPIO_ASYNC_RISING_EDGE_DETECT_EN_0 + (0x04 * regIdx)); \
        *gpio_asyn_rising_detect_reg = \
            ((*gpio_asyn_rising_detect_reg) | (0x01 << regShift)); \
    } while(0);

#define gpio_asyn_rising_edge_detect_disable(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_asyn_rising_detect_reg = \
            (volatile UINT32 *)(GPIO_ASYNC_RISING_EDGE_DETECT_EN_0 + (0x04 * regIdx)); \
        *gpio_asyn_rising_detect_reg = \
            ((*gpio_asyn_rising_detect_reg) & ~(0x01 << regShift)); \
    } while(0);

/*
The asynchronous falling edge detect enable registers define the pins for which a 
asynchronous falling edge transition sets a bit in the event detect status registers 
(GPEDSn). Asynchronous means the incoming signal is not sampled by the system 
clock. As such falling edges of very short duration can be detected
*/
#define gpio_asyn_falling_edge_detect_enable(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_asyn_falling_detect_reg = \
            (volatile UINT32 *)(GPIO_ASYNC_FALLING_EDGE_DETECT_EN_0 + (0x04 * regIdx)); \
        *gpio_asyn_falling_detect_reg = \
            ((*gpio_asyn_falling_detect_reg) | (0x01 << regShift)); \
    } while(0);

#define gpio_asyn_falling_edge_detect_disable(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_asyn_falling_detect_reg = \
            (volatile UINT32 *)(GPIO_ASYNC_FALLING_EDGE_DETECT_EN_0 + (0x04 * regIdx)); \
        *gpio_asyn_falling_detect_reg = \
            ((*gpio_asyn_falling_detect_reg) & ~(0x01 << regShift)); \
    } while(0);

/*
The GPIO Pull-up/down Register controls the actuation of the internal pull-up/down 
control line to ALL the GPIO pins. This register must be used in conjunction with the 2 
GPPUDCLKn registers. 
Note that it is not possible to read back the current Pull-up/down settings and so it is the 
users’ responsibility to ‘remember’ which pull-up/downs are active. The reason for this is 
that GPIO pull-ups are maintained even in power-down mode when the core is off, when 
all register contents is lost. 
The Alternate function table also has the pull state which is applied after a power down. 
*/
#define gpio_pud_set(pud) \
    do { \
        volatile UINT32 *gpio_pud = (volatile UINT32 *)GPIO_PULL_UPDOWN_EN; \
        *gpio_pud = (((*gpio_pud) & ~GPIO_PULL_UPDOWN_EN_MASK) | pud); \
    } while(0);

/*
 The GPIO Pull-up/down Clock Registers control the actuation of internal pull-downs on 
the respective GPIO pins. These registers must be used in conjunction with the GPPUD 
register to effect GPIO Pull-up/down changes. The following sequence of events is 
required: 
1. Write to GPPUD to set the required control signal (i.e. Pull-up or Pull-Down or neither 
to remove the current Pull-up/down) 
2. Wait 150 cycles – this provides the required set-up time for the control signal 
3. Write to GPPUDCLK0/1 to clock the control signal into the GPIO pads you wish to 
modify – NOTE only the pads which receive a clock will be modified, all others will 
retain their previous state. 
4. Wait 150 cycles – this provides the required hold time for the control signal 
5. Write to GPPUD to remove the control signal 
6. Write to GPPUDCLK0/1 to remove the clock 
*/
#define gpio_pud_clk(pin) \
    do { \
        UINT8 regIdx = pin / 32; \
        UINT8 regShift = (regIdx == 0)? pin : (pin - 32); \
        volatile UINT32 *gpio_pud_clk = \
            (volatile UINT32 *)(GPIO_PULL_UPDOWN_EN_CLK_0 + (0x04 * regIdx)); \
        *gpio_pud_clk = ((*gpio_pud_clk) & ~(0x01 << regShift)) | (0x01 << regShift); \
    } while(0);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif //__BCM2836_GPIO_H__ 
