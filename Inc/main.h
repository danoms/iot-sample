#include "utils.h"

#ifndef MAIN_H_
#define MAIN_H_

#define UNUSED(x) (void)(x)

#ifdef DEBUG
#define LOG(msg...) printf(msg);
#else
#define LOG(msg...) ;
#endif

#define LED(port, pin)                                                         \
  (struct led_t) {                                                             \
    .base = GPIO##port, .ahben_bit = RCC_AHB1ENR_GPIO##port##EN,               \
    .moder_mask = GPIO_MODER_MODER##pin,                                       \
    .moder_bit = GPIO_MODER_MODER##pin##_0, .set_bit = GPIO_BSRR_BS_##pin,     \
    .reset_bit = GPIO_BSRR_BR_##pin, .apin = pin,                              \
  }

int key_pressed();
void led_train(struct led_t leds[], int go_up, int speed);

#endif // MAIN_H_
