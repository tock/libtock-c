#include <stdbool.h>
#include <stdio.h>

#include <libtock-sync/services/alarm.h>
#include <libtock-sync/services/unit_test.h>
#include <libtock/interface/button.h>
#include <libtock/interface/led.h>
#include <libtock/interface/syscalls/button_syscalls.h>
#include <libtock/peripherals/gpio.h>



#define GPIO1_IN 0
#define GPIO1_OUT 1
#define GPIO0_IN 14
#define GPIO0_OUT 15

#define LED1_IN 6
#define LED2_IN 7
#define LED3_IN 8
#define LED4_IN 9

#define BUTTON1_OUT 2
#define BUTTON2_OUT 3
#define BUTTON3_OUT 4
#define BUTTON4_OUT 5

static int int_nr;
static int int_ctr;

bool test_setup(void) {
  if (libtock_gpio_enable_input(GPIO0_IN, libtock_pull_none) != 0) {
    return false;
  }
  if (libtock_gpio_clear(GPIO0_OUT) != 0) {
    return false;
  }
  if (libtock_gpio_enable_output(GPIO0_OUT) != 0) {
    return false;
  }
  if (libtock_gpio_enable_input(GPIO1_IN, libtock_pull_none) != 0) {
    return false;
  }
  if (libtock_gpio_clear(GPIO1_OUT) != 0) {
    return false;
  }
  if (libtock_gpio_enable_output(GPIO1_OUT) != 0) {
    return false;
  }
  if (libtock_gpio_enable_input(LED1_IN, libtock_pull_none) != 0) {
    return false;
  }
  if (libtock_gpio_enable_input(LED2_IN, libtock_pull_none) != 0) {
    return false;
  }
  if (libtock_gpio_enable_input(LED3_IN, libtock_pull_none) != 0) {
    return false;
  }
  if (libtock_gpio_enable_input(LED4_IN, libtock_pull_none) != 0) {
    return false;
  }
  if (libtock_gpio_enable_output(BUTTON1_OUT) != 0) {
    return false;
  }
  if (libtock_gpio_enable_output(BUTTON2_OUT) != 0) {
    return false;
  }
  if (libtock_gpio_enable_output(BUTTON3_OUT) != 0) {
    return false;
  }
  if (libtock_gpio_enable_output(BUTTON4_OUT) != 0) {
    return false;
  }
  /* Buttons are low active */
  libtock_gpio_set(BUTTON1_OUT);
  libtock_gpio_set(BUTTON2_OUT);
  libtock_gpio_set(BUTTON3_OUT);
  libtock_gpio_set(BUTTON4_OUT);
  int_nr  = -1;
  int_ctr = 0;
  return true;
}

void test_teardown(void) {
  libtock_gpio_disable_interrupt(GPIO0_IN);
  libtock_gpio_disable(GPIO0_IN);
  libtock_gpio_disable(GPIO0_OUT);
  libtock_gpio_disable_interrupt(GPIO1_IN);
  libtock_gpio_disable(GPIO1_IN);
  libtock_gpio_disable(GPIO1_OUT);
  libtock_gpio_disable(LED1_IN);
  libtock_gpio_disable(LED2_IN);
  libtock_gpio_disable(LED3_IN);
  libtock_gpio_disable(LED4_IN);
  libtock_led_off(0);
  libtock_led_off(1);
  libtock_led_off(2);
  libtock_led_off(3);
  libtock_gpio_set(BUTTON1_OUT);
  libtock_gpio_set(BUTTON2_OUT);
  libtock_gpio_set(BUTTON3_OUT);
  libtock_gpio_set(BUTTON4_OUT);
  libtock_button_command_disable_interrupt(0);
  libtock_button_command_disable_interrupt(1);
  libtock_button_command_disable_interrupt(2);
  libtock_button_command_disable_interrupt(3);
}

static bool test_basic_gpio0(void) {
  int val;
  CHECK(libtock_gpio_read(GPIO0_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_gpio_set(GPIO0_OUT) == 0);
  CHECK(libtock_gpio_read(GPIO0_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_clear(GPIO0_OUT) == 0);
  CHECK(libtock_gpio_read(GPIO0_IN, &val) == 0);
  CHECK(val == 0);
  return true;
}

static bool test_basic_gpio1(void) {
  int val;
  CHECK(libtock_gpio_read(GPIO1_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_gpio_set(GPIO1_OUT) == 0);
  CHECK(libtock_gpio_read(GPIO1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_clear(GPIO1_OUT) == 0);
  CHECK(libtock_gpio_read(GPIO1_IN, &val) == 0);
  CHECK(val == 0);
  return true;
}


static void test_button_callback(
  __attribute__ ((unused)) returncode_t ret,
  int                                   pin_num,
  __attribute__ ((unused)) bool         val) {
  int_nr   = pin_num;
  int_ctr += 1;
}

static void test_gpio_callback(
  uint32_t                      pin_num,
  __attribute__ ((unused)) bool val) {
  int_nr   = pin_num;
  int_ctr += 1;
}

static bool test_gpio0_int_raising(void) {
  int val;
  CHECK(libtock_gpio_set_interrupt_callback(test_gpio_callback) == 0);
  CHECK(libtock_gpio_enable_interrupt(GPIO0_IN, libtock_rising_edge) == 0);
  CHECK(libtock_gpio_set(GPIO0_OUT) == 0);
  CHECK(libtock_gpio_read(GPIO0_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_clear(GPIO0_OUT) == 0);
  CHECK(libtock_gpio_read(GPIO0_IN, &val) == 0);
  CHECK(val == 0);
  libtocksync_alarm_delay_ms(2);
  CHECK(int_ctr == 1);
  CHECK(int_nr == GPIO0_IN);
  return true;
}

static bool test_gpio0_int_falling(void) {
  CHECK(libtock_gpio_set_interrupt_callback(test_gpio_callback) == 0);
  CHECK(libtock_gpio_enable_interrupt(GPIO0_IN, libtock_falling_edge) == 0);
  CHECK(libtock_gpio_set(GPIO0_OUT) == 0);
  CHECK(libtock_gpio_clear(GPIO0_OUT) == 0);
  libtocksync_alarm_delay_ms(2);
  CHECK(int_ctr == 1);
  CHECK(int_nr == GPIO0_IN);
  return true;
}

static bool test_gpio0_int_both(void) {
  CHECK(libtock_gpio_set_interrupt_callback(test_gpio_callback) == 0);
  CHECK(libtock_gpio_enable_interrupt(GPIO0_IN, libtock_change) == 0);
  CHECK(libtock_gpio_set(GPIO0_OUT) == 0);
  CHECK(libtock_gpio_clear(GPIO0_OUT) == 0);
  libtocksync_alarm_delay_ms(2);
  CHECK(int_ctr == 2);
  CHECK(int_nr == GPIO0_IN);
  return true;
}

static bool test_gpio1_int_raising(void) {
  int val;
  CHECK(libtock_gpio_set_interrupt_callback(test_gpio_callback) == 0);
  CHECK(libtock_gpio_enable_interrupt(GPIO1_IN, libtock_rising_edge) == 0);
  CHECK(libtock_gpio_set(GPIO1_OUT) == 0);
  CHECK(libtock_gpio_read(GPIO1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_clear(GPIO1_OUT) == 0);
  CHECK(libtock_gpio_read(GPIO1_IN, &val) == 0);
  CHECK(val == 0);
  libtocksync_alarm_delay_ms(2);
  CHECK(int_ctr == 1);
  CHECK(int_nr == GPIO1_IN);
  return true;
}

static bool test_gpio1_int_falling(void) {
  CHECK(libtock_gpio_set_interrupt_callback(test_gpio_callback) == 0);
  CHECK(libtock_gpio_enable_interrupt(GPIO1_IN, libtock_falling_edge) == 0);
  CHECK(libtock_gpio_set(GPIO1_OUT) == 0);
  CHECK(libtock_gpio_clear(GPIO1_OUT) == 0);
  libtocksync_alarm_delay_ms(2);
  CHECK(int_ctr == 1);
  CHECK(int_nr == GPIO1_IN);
  return true;
}

static bool test_gpio1_int_both(void) {
  CHECK(libtock_gpio_set_interrupt_callback(test_gpio_callback) == 0);
  CHECK(libtock_gpio_enable_interrupt(GPIO1_IN, libtock_change) == 0);
  CHECK(libtock_gpio_set(GPIO1_OUT) == 0);
  CHECK(libtock_gpio_clear(GPIO1_OUT) == 0);
  libtocksync_alarm_delay_ms(2);
  CHECK(int_ctr == 2);
  CHECK(int_nr == GPIO1_IN);
  return true;
}

static bool test_leds_start_off(void) {
  int count, val;
  /* LED outputs are low active */
  CHECK(libtock_led_count(&count) == 0);
  CHECK(count == 4);
  CHECK(libtock_gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}

static bool test_switch_led1(void) {
  int val;
  CHECK(libtock_led_on(0) == 0);
  CHECK(libtock_gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_led_off(0) == 0);
  CHECK(libtock_gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}

static bool test_switch_led2(void) {
  int val;
  CHECK(libtock_led_on(1) == 0);
  CHECK(libtock_gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_led_off(1) == 0);
  CHECK(libtock_gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}

static bool test_switch_led3(void) {
  int val;
  CHECK(libtock_led_on(2) == 0);
  CHECK(libtock_gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_led_off(2) == 0);
  CHECK(libtock_gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}

static bool test_switch_led4(void) {
  int val;
  CHECK(libtock_led_on(3) == 0);
  CHECK(libtock_gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_led_off(3) == 0);
  CHECK(libtock_gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}

static bool test_toggle_led1(void) {
  int val;
  CHECK(libtock_led_toggle(0) == 0);
  CHECK(libtock_gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_led_toggle(0) == 0);
  CHECK(libtock_gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}

static bool test_toggle_led2(void) {
  int val;
  CHECK(libtock_led_toggle(1) == 0);
  CHECK(libtock_gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_led_toggle(1) == 0);
  CHECK(libtock_gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}

static bool test_toggle_led3(void) {
  int val;
  CHECK(libtock_led_toggle(2) == 0);
  CHECK(libtock_gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_led_toggle(2) == 0);
  CHECK(libtock_gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}

static bool test_toggle_led4(void) {
  int val;
  CHECK(libtock_led_toggle(3) == 0);
  CHECK(libtock_gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_led_toggle(3) == 0);
  CHECK(libtock_gpio_read(LED1_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED2_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED3_IN, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_read(LED4_IN, &val) == 0);
  CHECK(val == 1);
  return true;
}

static bool test_buttons_start_off(void) {
  int count, val;
  CHECK(libtock_button_count(&count) == 0);
  CHECK(count == 4);
  CHECK(libtock_button_read(0, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(1, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(2, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(3, &val) == 0);
  CHECK(val == 0);
  return true;
}

static bool test_push_button1(void) {
  int val;
  CHECK(libtock_gpio_clear(BUTTON1_OUT) == 0);
  CHECK(libtock_button_read(0, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_button_read(1, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(2, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(3, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_gpio_set(BUTTON1_OUT) == 0);
  CHECK(libtock_button_read(0, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(1, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(2, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(3, &val) == 0);
  CHECK(val == 0);
  return true;
}

static bool test_push_button2(void) {
  int val;
  CHECK(libtock_gpio_clear(BUTTON2_OUT) == 0);
  CHECK(libtock_button_read(0, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(1, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_button_read(2, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(3, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_gpio_set(BUTTON2_OUT) == 0);
  CHECK(libtock_button_read(0, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(1, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(2, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(3, &val) == 0);
  CHECK(val == 0);
  return true;
}

static bool test_push_button3(void) {
  int val;
  CHECK(libtock_gpio_clear(BUTTON3_OUT) == 0);
  CHECK(libtock_button_read(0, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(1, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(2, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_button_read(3, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_gpio_set(BUTTON3_OUT) == 0);
  CHECK(libtock_button_read(0, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(1, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(2, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(3, &val) == 0);
  CHECK(val == 0);
  return true;
}

static bool test_push_button4(void) {
  int val;
  CHECK(libtock_gpio_clear(BUTTON4_OUT) == 0);
  CHECK(libtock_button_read(0, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(1, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(2, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(3, &val) == 0);
  CHECK(val == 1);
  CHECK(libtock_gpio_set(BUTTON4_OUT) == 0);
  CHECK(libtock_button_read(0, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(1, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(2, &val) == 0);
  CHECK(val == 0);
  CHECK(libtock_button_read(3, &val) == 0);
  CHECK(val == 0);
  return true;
}

static bool test_button1_int(void) {
  CHECK(libtock_button_notify_on_press(0, test_button_callback) == 0);
  CHECK(libtock_gpio_clear(BUTTON1_OUT) == 0);
  libtocksync_alarm_delay_ms(2);
  CHECK(int_ctr == 1);
  CHECK(int_nr == 0);
  CHECK(libtock_gpio_set(BUTTON1_OUT) == 0);
  libtocksync_alarm_delay_ms(2);
  CHECK(int_ctr == 2);
  CHECK(int_nr == 0);
  return true;
}

static bool test_two_buttons_int(void) {
  CHECK(libtock_button_notify_on_press(0, test_button_callback) == 0);
  CHECK(libtock_button_notify_on_press(1, test_button_callback) == 0);
  CHECK(libtock_gpio_clear(BUTTON1_OUT) == 0);
  libtocksync_alarm_delay_ms(2);
  CHECK(int_ctr == 1);
  CHECK(int_nr == 0);
  CHECK(libtock_gpio_clear(BUTTON2_OUT) == 0);
  libtocksync_alarm_delay_ms(2);
  CHECK(int_ctr == 2);
  CHECK(int_nr == 1);
  CHECK(libtock_gpio_set(BUTTON2_OUT) == 0);
  libtocksync_alarm_delay_ms(2);
  CHECK(int_ctr == 3);
  CHECK(int_nr == 1);
  CHECK(libtock_gpio_set(BUTTON1_OUT) == 0);
  libtocksync_alarm_delay_ms(2);
  CHECK(int_ctr == 4);
  CHECK(int_nr == 0);
  return true;
}

static bool test_disable_button_int(void) {
  CHECK(libtock_button_notify_on_press(0, test_button_callback) == 0);
  CHECK(libtock_button_notify_on_press(1, test_button_callback) == 0);
  CHECK(libtock_gpio_clear(BUTTON1_OUT) == 0);
  libtocksync_alarm_delay_ms(2);
  CHECK(int_ctr == 1);
  CHECK(int_nr == 0);
  CHECK(libtock_gpio_clear(BUTTON2_OUT) == 0);
  libtocksync_alarm_delay_ms(2);
  CHECK(int_ctr == 2);
  CHECK(int_nr == 1);
  CHECK(libtock_button_command_disable_interrupt(1) == 0);
  CHECK(libtock_gpio_set(BUTTON2_OUT) == 0);
  libtocksync_alarm_delay_ms(2);
  CHECK(int_ctr == 2);
  CHECK(libtock_gpio_set(BUTTON1_OUT) == 0);
  libtocksync_alarm_delay_ms(2);
  CHECK(int_ctr == 3);
  CHECK(int_nr == 0);
  return true;
}

int main(void) {
  unit_test_fun tests[] = {
    TEST(basic_gpio0),
    TEST(basic_gpio1),
    TEST(gpio0_int_raising),
    TEST(gpio0_int_falling),
    TEST(gpio0_int_both),
    TEST(gpio1_int_raising),
    TEST(gpio1_int_falling),
    TEST(gpio1_int_both),
    TEST(leds_start_off),
    TEST(switch_led1),
    TEST(switch_led2),
    TEST(switch_led3),
    TEST(switch_led4),
    TEST(toggle_led1),
    TEST(toggle_led2),
    TEST(toggle_led3),
    TEST(toggle_led4),
    TEST(buttons_start_off),
    TEST(push_button1),
    TEST(push_button2),
    TEST(push_button3),
    TEST(push_button4),
    TEST(button1_int),
    TEST(two_buttons_int),
    TEST(disable_button_int),
  };
  unit_test_runner(tests, sizeof(tests) / sizeof(unit_test_fun), 100, "org.tockos.unit_test");
  return 0;
}
