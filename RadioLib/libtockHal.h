/*
  RadioLib Non-Arduino Tock Library helper functions

  Licensed under the MIT License

  Copyright (c) 2023 Alistair Francis <alistair@alistair23.me>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#ifndef TOCK_RADIOLIB_HAL_H
#define TOCK_RADIOLIB_HAL_H

// include RadioLib
#include <RadioLib.h>

// include all the dependencies
#include "libtock/net/lora_phy.h"
#include "libtock/net/syscalls/lora_phy_syscalls.h"
#include "libtock-sync/net/lora_phy.h"
#include "libtock/peripherals/gpio.h"
#include "libtock-sync/services/alarm.h"
#include "libtock/kernel/read_only_state.h"

#define RADIOLIB_RADIO_BUSY   1
#define RADIOLIB_RADIO_DIO_1  2
#define RADIOLIB_RADIO_DIO_3  3
#define RADIOLIB_RADIO_RESET  4
// Skip the chips select as Tock handles this for us
#define RADIOLIB_RADIO_NSS   RADIOLIB_NC

// define Arduino-style macros
#define TOCK_RADIOLIB_PIN_LOW           (0x0)
#define TOCK_RADIOLIB_PIN_HIGH          (0x1)
#define TOCK_RADIOLIB_PIN_INPUT         (0x01)
#define TOCK_RADIOLIB_PIN_OUTPUT        (0x03)
#define TOCK_RADIOLIB_PIN_RISING        (0x01)
#define TOCK_RADIOLIB_PIN_FALLING       (0x02)

typedef void (*gpioIrqFn)(void);

gpioIrqFn gpio_funcs[4] = { NULL, NULL, NULL, NULL};

static void lora_phy_gpio_Callback (int gpioPin,
                                    __attribute__ ((unused)) int arg2,
                                    __attribute__ ((unused)) int arg3,
                                    void* userdata)
{
    gpioIrqFn fn = gpio_funcs[gpioPin - 1];

    if (fn != NULL ) {
        fn();
    }
}

class TockRadioLibHal : public RadioLibHal {
  public:
    // default constructor - initializes the base HAL and any needed private members
    TockRadioLibHal()
      : RadioLibHal(TOCK_RADIOLIB_PIN_INPUT, TOCK_RADIOLIB_PIN_OUTPUT, TOCK_RADIOLIB_PIN_LOW, TOCK_RADIOLIB_PIN_HIGH, TOCK_RADIOLIB_PIN_RISING, TOCK_RADIOLIB_PIN_FALLING)  {
    }

    void init() override {
    }

    void term() override {
    }

    // GPIO-related methods (pinMode, digitalWrite etc.) should check
    // RADIOLIB_NC as an alias for non-connected pins
    void pinMode(uint32_t pin, uint32_t mode) override {
      if(pin == RADIOLIB_NC) {
        return;
      }

      if (mode == TOCK_RADIOLIB_PIN_OUTPUT) {
          libtock_lora_phy_gpio_enable_output(pin);
      } else if (mode == TOCK_RADIOLIB_PIN_INPUT) {
          libtock_lora_phy_gpio_enable_input(pin, libtock_pull_down);
      }
    }

    void digitalWrite(uint32_t pin, uint32_t value) override {
      if(pin == RADIOLIB_NC) {
        return;
      }

      if (value) {
          libtock_lora_phy_gpio_set(pin);
      } else {
          libtock_lora_phy_gpio_clear(pin);
      }
    }

    uint32_t digitalRead(uint32_t pin) override {
      int value;

      if(pin == RADIOLIB_NC) {
        return 0;
      }

      libtock_lora_phy_gpio_read(pin, &value);

      return value;
    }

    void attachInterrupt(uint32_t interruptNum, gpioIrqFn interruptCb, uint32_t mode) override {
      if(interruptNum == RADIOLIB_NC) {
        return;
      }

      gpio_funcs[interruptNum - 1] = interruptCb;
      libtock_lora_phy_gpio_command_interrupt_callback(lora_phy_gpio_Callback, NULL);

      // set GPIO as input and enable interrupts on it
      libtock_lora_phy_gpio_enable_input(interruptNum, libtock_pull_down);
      libtock_lora_phy_gpio_enable_interrupt(interruptNum, libtock_change);
    }

    void detachInterrupt(uint32_t interruptNum) override {
      if(interruptNum == RADIOLIB_NC) {
        return;
      }

      gpio_funcs[interruptNum - 1] = NULL;
      libtock_lora_phy_gpio_disable_interrupt(interruptNum);
      libtock_lora_phy_gpio_enable_input(interruptNum, libtock_pull_down);
    }

    void delay(unsigned long ms) override {
#if !defined(RADIOLIB_CLOCK_DRIFT_MS)
      libtocksync_alarm_delay_ms(ms);
#else
      libtocksync_alarm_delay_ms(ms * 1000 / (1000 + RADIOLIB_CLOCK_DRIFT_MS));
#endif
    }

    void delayMicroseconds(unsigned long us) override {
#if !defined(RADIOLIB_CLOCK_DRIFT_MS)
      libtocksync_alarm_delay_ms(us / 1000);
#else
      libtocksync_alarm_delay_ms((us * 1000 / (1000 + RADIOLIB_CLOCK_DRIFT_MS)) / 1000);
#endif
    }

    unsigned long millis() override {
      struct timeval tv;
      unsigned long ms;

      libtock_alarm_gettimeasticks(&tv);
      ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;

#if !defined(RADIOLIB_CLOCK_DRIFT_MS)
      return ms;
#else
      return (uint64_t)ms * 1000 / (1000 + RADIOLIB_CLOCK_DRIFT_MS);
#endif
    }

    unsigned long micros() override {
      struct timeval tv;

      libtock_alarm_gettimeasticks(&tv);
      return tv.tv_sec * 1000000 + tv.tv_usec;
    }

    long pulseIn(uint32_t pin, uint32_t state, unsigned long timeout) override {
      return 0;
    }

    void spiBegin() {
    }

    void spiBeginTransaction() {
    }

    void spiTransfer(uint8_t* out, size_t len, uint8_t* in) {
      libtocksync_lora_phy_read_write(out, in, len);
    }

    void spiEndTransaction() {
    }

    void spiEnd() {
    }

    void yield() {
      ::yield_no_wait();
    }

  private:
};

#endif
