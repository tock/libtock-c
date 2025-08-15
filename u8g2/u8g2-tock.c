#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include <u8g2.h>

#include <libtock-sync/display/screen.h>

#include "u8g2-tock.h"

// Copy of the page size function from the u8g2_buffer.c file we can't include.
static size_t page_size_bytes(u8g2_t* u8g2) {
  size_t cnt;
  cnt = u8g2_GetU8x8(u8g2)->display_info->tile_width;
  cnt *= u8g2->tile_buf_height;
  cnt *= 8;
  return cnt;
}

// Default display_info struct used to interface with the library. We modify
// this when we learn about the screen from the kernel.
static u8x8_display_info_t u8x8_ssd1306_tock =
{
  /* NOT RELEVANT FOR TOCK */
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  /* post_chip_enable_wait_ns = */ 20,
  /* pre_chip_disable_wait_ns = */ 10,
  /* reset_pulse_width_ms = */ 100,   /* SSD1306: 3 us */
  /* post_reset_wait_ms = */ 100, /* far east OLEDs need much longer setup time */
  /* sda_setup_time_ns = */ 50,   /* SSD1306: 15ns, but cycle time is 100ns, so use 100/2 */
  /* sck_pulse_width_ns = */ 50,  /* SSD1306: 20ns, but cycle time is 100ns, so use 100/2, AVR: below 70: 8 MHz, >= 70 --> 4MHz clock */
  /* sck_clock_hz = */ 8000000UL, /* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 0,   /* active high, rising edge */
  /* i2c_bus_clock_100kHz = */ 4,
  /* data_setup_time_ns = */ 40,
  /* write_pulse_width_ns = */ 150, /* SSD1306: cycle time is 300ns, so use 300/2 = 150 */

  /* BASED ON THE DISPLAY */
  /* tile_width = */ 16,
  /* tile_height = */ 8,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 128,
  /* pixel_height = */ 64
};

static uint8_t u8x8_d_ssd1306_tock(u8x8_t *u8x8,
                            uint8_t msg,
                            __attribute__ ((unused)) uint8_t arg_int,
                            __attribute__ ((unused)) void *arg_ptr) {

  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY: {

      // Update the default display_info with actual information from the
      // kernel.
      uint32_t width = 0, height = 0;
      libtock_screen_get_resolution(&width, &height);
      u8x8_ssd1306_tock.tile_width = width/8;
      u8x8_ssd1306_tock.tile_height = height/8;
      u8x8_ssd1306_tock.pixel_width = width;
      u8x8_ssd1306_tock.pixel_height = height;

      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ssd1306_tock);
      break;
    }
    default:
      return 0;
  }
  return 1;
}

// Initialize the u8g2 library for Tock use. Call this before using the rest of
// the library.
int u8g2_tock_init(u8g2_t *u8g2) {
  if (!libtocksync_screen_exists()) {
    return -1;
  }

  // Since we use syscalls mostly don't need this. Just need to setup
  // display_info.
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_tock, u8x8_dummy_cb, u8x8_dummy_cb, u8x8_dummy_cb);

  // Allocate the buffer for the screen.
  size_t screen_bytes = (u8g2_GetU8x8(u8g2)->display_info->pixel_width * u8g2_GetU8x8(u8g2)->display_info->pixel_height) / 8;
  if (screen_bytes == 0) {
    return -1;
  }
  uint8_t* buf = (uint8_t*) calloc(1, screen_bytes);

  // Setup the u8g2 struct.
  uint8_t tile_buf_height = u8g2_GetU8x8(u8g2)->display_info->tile_height;
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, U8G2_R0);

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// u8g2_buffer.c functions
////////////////////////////////////////////////////////////////////////////////

// Clear buffer since we don't include u8g2_buffer.c in the library.
void u8g2_ClearBuffer(u8g2_t *u8g2) {
  memset(u8g2->tile_buf_ptr, 0, page_size_bytes(u8g2));
}

// Send entire buffer via Tock syscalls.
void u8g2_SendBuffer(u8g2_t *u8g2) {

  // Set the frame to the entire region.
  uint16_t width = u8g2_GetU8x8(u8g2)->display_info->pixel_width;
  uint16_t height = u8g2_GetU8x8(u8g2)->display_info->pixel_height;
  libtocksync_screen_set_frame(0, 0, width, height);

  // Write the data to the screen.
  libtocksync_screen_write(u8g2->tile_buf_ptr, page_size_bytes(u8g2), page_size_bytes(u8g2));
}



void u8g2_SetBufferCurrTileRow(u8g2_t *u8g2, uint8_t row)
{
  u8g2->tile_curr_row = row;
  u8g2->cb->update_dimension(u8g2);
  u8g2->cb->update_page_win(u8g2);
}

void u8g2_FirstPage(u8g2_t *u8g2)
{
  if ( u8g2->is_auto_page_clear )
  {
    u8g2_ClearBuffer(u8g2);
  }
  u8g2_SetBufferCurrTileRow(u8g2, 0);
}

uint8_t u8g2_NextPage(u8g2_t *u8g2)
{
  uint8_t row;
  u8g2_SendBuffer(u8g2);
  row = u8g2->tile_curr_row;
  row += u8g2->tile_buf_height;
  if ( row >= u8g2_GetU8x8(u8g2)->display_info->tile_height )
  {
    u8x8_RefreshDisplay( u8g2_GetU8x8(u8g2) );
    return 0;
  }
  if ( u8g2->is_auto_page_clear )
  {
    u8g2_ClearBuffer(u8g2);
  }
  u8g2_SetBufferCurrTileRow(u8g2, row);
  return 1;
}

