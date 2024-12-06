#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <libtock/interface/button.h>

uint8_t global_temperature_setpoint = 0;
uint8_t local_temperature_setpoint  = 22;
uint8_t measured_temperature        = 0;


static void button_callback(returncode_t ret,
                            int          btn_num,
                            bool         pressed) {

  if (pressed) {
    printf("Button %i pressed!\r\n", btn_num);
  }
}

int main(void) {
  for (int i = 0; i < 4; i++){
    libtock_button_notify_on_press(i, button_callback);
  }

  update_screen();

  for(;;) {
    yield();
  }  
}

static void update_screen(void) {
  char temperature_set_point_str[35];
  char temperature_global_set_point_str[35];
  char temperature_current_measure_str[35];
  
  // Format the buffers to be written.
  sprintf(temperature_set_point_str,
          "Set Point: %d", 
	  local_temperature_setpoint);

  sprintf(temperature_global_set_point_str, 
          "Global Set Point: %d",
	  global_temperature_setpoint);

  sprintf(temperature_current_measure_str, 
	  "Measured Temp: %d",
	  measured_temperature);

  // Use u8g2 library to draw each string.
  u8g2_ClearBuffer(&u8g2);
  u8g2_SetDrawColor(&u8g2, 1);
  u8g2_DrawStr(&u8g2, 0, 0, temperature_set_point_str);
  u8g2_DrawStr(&u8g2, 0, 25, temperature_global_set_point_str);
  u8g2_DrawStr(&u8g2, 0, 50, temperature_current_measure_str);
  u8g2_SendBuffer(&u8g2);
}

