#include "utils.h"

// Quick and dirty delay
void delay(unsigned int time) {
  for (unsigned int i = 0; i < time; i++)
    for (volatile unsigned int j = 0; j < 2000; j++)
      ;
}

int key_pressed(GPIO_TypeDef *base, int pin) {
  static int was_pressed = 0;
  int key_released = base->IDR & (1 << pin);
  if (!(key_released) && was_pressed) {
    return 0;
  } else {
    was_pressed = 0;
    if (!(key_released)) {
      was_pressed = 1;
      return 1;
    } else {
      return 0;
    }
  }
}

void int_to_bin(int num, char *buffer, int size) {
  for (int i = 0; i < size; i++) {
    if ((num) & (1 << i)) {
      buffer[i] = 0x31;
    } else {
      buffer[i] = 0x30;
    }
  }
  buffer[size] = 0;
}

int uart_prompt(char *prompt) {
  char rxb = '\0';
  char buffer[10];
  int count = 0;
  int option = 0;

  printf("%s", prompt);
  fflush(stdout);
  while (rxb != 0xD && count < 10) {
    while (!(USART6->ISR & USART_ISR_RXNE)) {
    };
    rxb = USART6->RDR;
    buffer[count] = rxb;
    count++;

    printf("%c", rxb);
    fflush(stdout);
  }
  printf("\r\n");
  option = atoi(buffer);
  return option;
}

int led_pins_state(struct led_t leds[], int size) {
  int state = 0;
  for (int i = 0; i < size; i++) {
    if (leds[i].base->ODR & (1 << leds[i].apin)) {
      state |= 1 << i;
    }
  }
  return state;
}
