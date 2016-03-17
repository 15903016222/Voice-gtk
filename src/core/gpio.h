/**
 * @file gpio.h
 * @brief gpio operation
 * @author JakeYang <add358@gmail.com>
 * @version 0.1
 * @date 2016-03-16
 */

#ifndef __GPIO_H__
#define __GPIO_H__

typedef struct _Gpio Gpio;

typedef enum {
    GPIO_43 = 0x6002,
    GPIO_61 = 0x6000
}GpioPin;

typedef enum {
    PIN_LOW = 0,
    PIN_HIGH
}PinState;

extern int gpio_init();
extern void gpio_uninit();

extern int gpio_set_pin(GpioPin pin, PinState state);

#endif /* end of include guard */
