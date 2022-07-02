#include <stdint.h>
#include <stdio.h>

#include <internal/ota.h>
#include <button.h>
#include <timer.h>

#define BUFFER_SIZE 512
#define START_ADDRESS_OF_APPLICATION 0x00044000 // It has to be modified accroding to OTA app size
#define END_ADDRESS_OF_FLASH 0x00080000
//#define CRC8_POLYNOMIAL 0x31

#if 0
static void test_main(void);
static int test_all(void);
static int test(uint8_t *readbuf, uint8_t *writebuf, size_t size, size_t offset, size_t len);

static void test_main(void)
{
  int r = test_all();
  if (r == 0)
  {
    printf("All tests succeeded\n");
  }
  else
  {
    printf("Failed with code %d\n", r);
  }
}

static int test_all(void)
{
  int num_bytes;
  ota_userspace_size_check(&num_bytes);
  printf("Have %i bytes (%i kb)of nonvolatile storage\n", num_bytes, (num_bytes / 1024));

  int r;
  uint8_t readbuf[512];
  uint8_t writebuf[512];

  // if ((r = test(readbuf, writebuf, 256, 0,  14)) != 0) return r;
  // if ((r = test(readbuf, writebuf, 256, 20, 14)) != 0) return r;
  if ((r = test(readbuf, writebuf, 512, 0, 512)) != 0)
    return r;

  return 0;
}

static int test(uint8_t *readbuf, uint8_t *writebuf, size_t size, size_t offset, size_t len)
{
  int ret;

  printf("[Nonvolatile Storage] Test App\n");
  printf("Test with size %d ...\n", size);

  ret = ota_internal_read_buffer(readbuf, size);
  if (ret != RETURNCODE_SUCCESS)
  {
    printf("\tERROR setting read buffer\n");
    return ret;
  }

  ret = ota_internal_write_buffer(writebuf, size);
  if (ret != RETURNCODE_SUCCESS)
  {
    printf("\tERROR setting write buffer\n");
    return ret;
  }

  printf("==============write data===================\n");
  size_t prnt_cnt = 0;
  for (size_t i = 0; i < len; i++)
  {
    writebuf[i] = i;
    prnt_cnt += 1;
    printf("%x, ", writebuf[i]);
    if (prnt_cnt == 12)
    {
      prnt_cnt = 0;
      printf("\n");
    }
  }

  done = false;
  ret = ota_data_write_execution(offset, len);
  if (ret != 0)
  {
    printf("\tERROR calling write\n");
    return ret;
  }
  yield_for(&done);

  done = false;
  ret = ota_data_read_execution(offset, len);
  if (ret != 0)
  {
    printf("\tERROR calling read\n");
    return ret;
  }
  yield_for(&done);

  printf("==============read data===================\n");
  prnt_cnt = 0;
  for (size_t i = 0; i < len; i++)
  {
    readbuf[i] = i;
    prnt_cnt += 1;
    printf("%x, ", readbuf[i]);
    if (prnt_cnt == 12)
    {
      prnt_cnt = 0;
      printf("\n");
    }
  }

  for (size_t i = 0; i < len; i++)
  {
    if (readbuf[i] != writebuf[i])
    {
      printf("\tInconsistency between data written and read at index %u\n", i);
      return -1;
    }
  }

  return 0;
}
#endif

int callback_subscribe_entry_initialzation(void);
int button_init(void);
void blink_binary_write(void);

static bool done = false;

uint8_t blkapp_bin[] = {0x2, 0x0, 0x34, 0x0, 0x0, 0x8, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0xD7, 0x75, 0x50, 0x6E,
                        0x1, 0x0, 0x0C, 0x0, 0x29, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFC, 0x11, 0x0, 0x0,
                        0x3, 0x0, 0x5, 0x0, 0x62, 0x6C, 0x69, 0x6E, 0x6B, 0x0, 0x0, 0x0, 0x8, 0x0, 0x4, 0x0,
                        0x2, 0x0, 0x0, 0x0, 0xD8, 0x5, 0x0, 0x0, 0x0, 0x8, 0x0, 0x0, 0x2C, 0x0, 0x0, 0x0,
                        0x4, 0x6, 0x0, 0x0, 0x2C, 0x8, 0x0, 0x0, 0x70, 0x0, 0x0, 0x0, 0x9C, 0x8, 0x0, 0x0,
                        0x60, 0x1, 0x0, 0x0, 0x74, 0x6, 0x0, 0x0, 0x0, 0x8, 0x0, 0x0, 0x44, 0x6A, 0x4, 0xF1,
                        0x7, 0x4, 0x0C, 0x44, 0x7, 0x25, 0x24, 0xEA, 0x5, 0x4, 0x85, 0x69, 0xC6, 0x69, 0x35, 0x44,
                        0x0D, 0x44, 0x6, 0x0, 0x0F, 0x0, 0x9D, 0x42, 0x0, 0xDC, 0xA5, 0x46, 0x0, 0x20, 0x29, 0x0,
                        0x5, 0xDF, 0xA5, 0x46, 0x0A, 0x20, 0x21, 0x0, 0x5, 0xDF, 0x0B, 0x20, 0x29, 0x0, 0x5, 0xDF,
                        0x70, 0x68, 0x38, 0x44, 0x81, 0x46, 0x30, 0x0, 0x39, 0x0, 0x0, 0xF0, 0x1F, 0xF8, 0xFF, 0xBE,
                        0x37, 0xB5, 0x1, 0xA8, 0x0, 0xF0, 0xD5, 0xF9, 0x0, 0x28, 0x15, 0xDB, 0x0, 0x25, 0x0, 0x24,
                        0x1, 0x9B, 0xA3, 0x42, 0x4, 0xDC, 0xFA, 0x20, 0x0, 0xF0, 0xE0, 0xF8, 0x1, 0x35, 0xF6, 0xE7,
                        0x45, 0xFA, 0x4, 0xF3, 0xDB, 0x7, 0x20, 0x46, 0x3, 0xD5, 0x0, 0xF0, 0xD4, 0xF9, 0x1, 0x34,
                        0xEE, 0xE7, 0x0, 0xF0, 0xE2, 0xF9, 0xFA, 0xE7, 0x3, 0xB0, 0x30, 0xBD, 0x0D, 0x46, 0x8, 0xB5,
                        0x41, 0x68, 0x3, 0x68, 0x4, 0x46, 0x29, 0x44, 0x18, 0x44, 0x0, 0x22, 0xA3, 0x68, 0xB2, 0xEB,
                        0x93, 0x0F, 0x19, 0xD3, 0xD4, 0xE9, 0x3, 0x10, 0x62, 0x69, 0x21, 0x44, 0x28, 0x44, 0x0, 0xF0,
                        0x3D, 0xFA, 0xD4, 0xE9, 0x6, 0x2, 0x0, 0x21, 0x28, 0x44, 0x0, 0xF0, 0x45, 0xFA, 0x21, 0x6A,
                        0x21, 0x44, 0x0, 0x22, 0x0E, 0x1D, 0x0B, 0x68, 0xB2, 0xEB, 0x93, 0x0F, 0x10, 0xD3, 0xFF, 0xF7,
                        0xBF, 0xFF, 0x0, 0xF0, 0x5D, 0xF9, 0xFC, 0xE7, 0x50, 0xF8, 0x22, 0x30, 0x0, 0x2B, 0xB6, 0xBF,
                        0x3, 0xF1, 0x0, 0x43, 0x5B, 0x19, 0x1B, 0x19, 0x41, 0xF8, 0x22, 0x30, 0x1, 0x32, 0xD5, 0xE7,
                        0x56, 0xF8, 0x22, 0x0, 0x2B, 0x58, 0x0, 0x2B, 0xB6, 0xBF, 0x3, 0xF1, 0x0, 0x43, 0x5B, 0x19,
                        0x1B, 0x19, 0x2B, 0x50, 0x2, 0x32, 0xDE, 0xE7, 0x1, 0x22, 0x1A, 0x70, 0x70, 0x47, 0xFF, 0xFF,
                        0xF0, 0xB5, 0x15, 0x4B, 0x59, 0xF8, 0x3, 0x50, 0x85, 0xB0, 0x2C, 0x68, 0x3, 0xAF, 0x0, 0x26,
                        0x6C, 0xB1, 0x38, 0x46, 0x0, 0xF0, 0xD8, 0xF9, 0xDD, 0xF8, 0x0C, 0xC0, 0x22, 0x68, 0x61, 0x68,
                        0xAC, 0xEB, 0x2, 0x3, 0x99, 0x42, 0x4, 0xD9, 0x10, 0x46, 0x0, 0xF0, 0xA9, 0xF9, 0x5, 0xB0,
                        0xF0, 0xBD, 0x2B, 0x68, 0x23, 0xB1, 0x18, 0x69, 0x28, 0x60, 0x0, 0xB1, 0x46, 0x61, 0x1E, 0x61,
                        0xA0, 0x68, 0x30, 0xB1, 0xE3, 0x68, 0x0, 0x93, 0x0A, 0x44, 0x0, 0x23, 0x61, 0x46, 0x0, 0xF0,
                        0x7D, 0xF8, 0x2C, 0x68, 0xDC, 0xE7, 0x0, 0xBF, 0x8, 0x0, 0x0, 0x0, 0xF8, 0xB5, 0x6, 0x9C,
                        0x0, 0x25, 0xC4, 0xE9, 0x2, 0x23, 0xC4, 0xE9, 0x0, 0x1, 0xC4, 0xE9, 0x4, 0x55, 0x18, 0x4B,
                        0x59, 0xF8, 0x3, 0x60, 0x33, 0x68, 0x63, 0xB9, 0x34, 0x60, 0x16, 0x4B, 0x0, 0x21, 0x59, 0xF8,
                        0x3, 0x0, 0x0, 0xF0, 0x6C, 0xF9, 0xD4, 0xE9, 0x0, 0x1, 0xBD, 0xE8, 0xF8, 0x40, 0x0, 0xF0,
                        0x77, 0xB9, 0xB4, 0x46, 0x1, 0x44, 0xD3, 0xE9, 0x0, 0x27, 0x3A, 0x44, 0x17, 0x1A, 0x52, 0x1A,
                        0x97, 0x42, 0x0B, 0xD3, 0xCC, 0xF8, 0x0, 0x40, 0xC4, 0xE9, 0x4, 0x35, 0x5C, 0x61, 0x33, 0x68,
                        0x9C, 0x42, 0xE2, 0xD0, 0x0, 0x20, 0xF8, 0xBD, 0x13, 0x46, 0xEC, 0xE7, 0x1A, 0x69, 0x3, 0xF1,
                        0x10, 0x0C, 0x1D, 0x46, 0x0, 0x2A, 0xF7, 0xD1, 0x1C, 0x61, 0xC4, 0xE9, 0x4, 0x23, 0xEE, 0xE7,
                        0x8, 0x0, 0x0, 0x0, 0x4, 0x0, 0x0, 0x0, 0xF0, 0xB5, 0x85, 0xB0, 0x4, 0x46, 0x2, 0xA8,
                        0x17, 0x46, 0x0E, 0x46, 0x1D, 0x46, 0x0, 0xF0, 0x5D, 0xF9, 0x2, 0x98, 0x4F, 0xF4, 0x7A, 0x73,
                        0xB4, 0xFB, 0xF3, 0xF1, 0x3, 0xFB, 0x11, 0x44, 0xB0, 0xFB, 0xF3, 0xF3, 0x48, 0x43, 0x3, 0xFB,
                        0x4, 0x4, 0x0C, 0x35, 0x3, 0xA8, 0x0, 0xF0, 0x5F, 0xF9, 0x3, 0x98, 0x0, 0x95, 0x3B, 0x46,
                        0x32, 0x46, 0x21, 0x46, 0xFF, 0xF7, 0xA2, 0xFF, 0x5, 0xB0, 0xF0, 0xBD, 0x30, 0xB5, 0x8B, 0xB0,
                        0x0, 0x23, 0x8D, 0xF8, 0x3, 0x30, 0x8, 0x4B, 0x0D, 0xF1, 0x3, 0x5, 0x59, 0xF8, 0x3, 0x10,
                        0x2A, 0x46, 0x1, 0xAB, 0xFF, 0xF7, 0xD0, 0xFF, 0x4, 0x46, 0x10, 0xB9, 0x28, 0x46, 0x0, 0xF0,
                        0xA7, 0xF8, 0x20, 0x46, 0x0B, 0xB0, 0x30, 0xBD, 0x0, 0x0, 0x0, 0x0, 0xF0, 0xB5, 0x7, 0x46,
                        0x13, 0x48, 0x59, 0xF8, 0x0, 0xC0, 0xDC, 0xF8, 0x0, 0xE0, 0x0E, 0xF1, 0x1, 0x6, 0x74, 0x42,
                        0x4, 0xF0, 0x0F, 0x4, 0x6, 0xF0, 0x0F, 0x0, 0x58, 0xBF, 0x60, 0x42, 0x0D, 0x4C, 0x59, 0xF8,
                        0x4, 0x40, 0x24, 0x68, 0x84, 0x42, 0x0F, 0xD0, 0x0B, 0x4C, 0x14, 0x25, 0x59, 0xF8, 0x4, 0x60,
                        0xCC, 0xF8, 0x0, 0x0, 0x5, 0xFB, 0x0E, 0xF5, 0x74, 0x19, 0xC4, 0xE9, 0x1, 0x12, 0xE3, 0x60,
                        0x77, 0x51, 0x5, 0x9B, 0x23, 0x61, 0xF0, 0xBD, 0x4F, 0xF0, 0xFF, 0x30, 0xFB, 0xE7, 0x0, 0xBF,
                        0x10, 0x0, 0x0, 0x0, 0x0C, 0x0, 0x0, 0x0, 0x14, 0x0, 0x0, 0x0, 0x40, 0x42, 0x70, 0x47,
                        0x84, 0xB0, 0x0D, 0xF1, 0x10, 0x0C, 0x0C, 0xE9, 0x0F, 0x0, 0x9D, 0xF8, 0x0, 0x30, 0x80, 0x2B,
                        0x5, 0xD0, 0x3B, 0xB9, 0x9D, 0xF8, 0x4, 0x0, 0x4, 0xB0, 0xFF, 0xF7, 0xEF, 0xBF, 0x0, 0x20,
                        0x4, 0xB0, 0x70, 0x47, 0x0, 0x48, 0xFB, 0xE7, 0x0, 0xFC, 0xFF, 0xFF, 0x84, 0xB0, 0x0D, 0xF1,
                        0x10, 0x0C, 0x0C, 0xE9, 0x0F, 0x0, 0x9D, 0xF8, 0x0, 0x30, 0x4, 0x9A, 0x1, 0x98, 0x81, 0x2B,
                        0x3, 0xD1, 0x10, 0x60, 0x0, 0x20, 0x4, 0xB0, 0x70, 0x47, 0x1B, 0xB9, 0xC0, 0xB2, 0x4, 0xB0,
                        0xFF, 0xF7, 0xD4, 0xBF, 0x0, 0x48, 0xF6, 0xE7, 0x0, 0xFC, 0xFF, 0xFF, 0x84, 0xB0, 0x0D, 0xF1,
                        0x10, 0x0C, 0x0C, 0xE9, 0x0F, 0x0, 0x9D, 0xF8, 0x0, 0x30, 0x23, 0xB9, 0x9D, 0xF8, 0x0C, 0x0,
                        0x4, 0xB0, 0xFF, 0xF7, 0xC3, 0xBF, 0x0, 0x20, 0x4, 0xB0, 0x70, 0x47, 0x10, 0xB5, 0x11, 0x4B,
                        0x11, 0x4A, 0x59, 0xF8, 0x3, 0x10, 0x59, 0xF8, 0x2, 0x20, 0x0B, 0x68, 0x12, 0x68, 0x93, 0x42,
                        0x16, 0xD0, 0x0E, 0x4A, 0x59, 0xF8, 0x2, 0x40, 0x14, 0x22, 0x5A, 0x43, 0x1, 0x33, 0xA0, 0x18,
                        0xA4, 0x58, 0x5A, 0x42, 0x2, 0xF0, 0x0F, 0x2, 0x3, 0xF0, 0x0F, 0x3, 0x58, 0xBF, 0x53, 0x42,
                        0x0B, 0x60, 0xD0, 0xE9, 0x3, 0x23, 0xD0, 0xE9, 0x1, 0x1, 0xA0, 0x47, 0x1, 0x20, 0x10, 0xBD,
                        0x0, 0x20, 0xFC, 0xE7, 0x0C, 0x0, 0x0, 0x0, 0x10, 0x0, 0x0, 0x0, 0x14, 0x0, 0x0, 0x0,
                        0x8, 0xB5, 0xFF, 0xF7, 0xD3, 0xFF, 0x1, 0x46, 0x8, 0xB9, 0x1, 0x20, 0x0, 0xDF, 0x8, 0xBD,
                        0x10, 0xB5, 0x4, 0x46, 0x23, 0x78, 0x3, 0xB1, 0x10, 0xBD, 0xFF, 0xF7, 0xF1, 0xFF, 0xF9, 0xE7,
                        0x10, 0xB5, 0x4, 0x46, 0x8, 0x46, 0x11, 0x46, 0x1A, 0x46, 0x2, 0x9B, 0x1, 0xDF, 0x82, 0x28,
                        0x7, 0xD1, 0x1, 0x23, 0xC4, 0xE9, 0x1, 0x12, 0x23, 0x70, 0x0, 0x23, 0x23, 0x73, 0x20, 0x46,
                        0x10, 0xBD, 0x2, 0x28, 0x5, 0xD1, 0x0, 0x20, 0xC4, 0xE9, 0x1, 0x23, 0x20, 0x70, 0x21, 0x73,
                        0xF5, 0xE7, 0x1, 0x20, 0x0, 0xF0, 0x8A, 0xF8, 0x10, 0xB5, 0x4, 0x46, 0x8, 0x46, 0x11, 0x46,
                        0x1A, 0x46, 0x2, 0x9B, 0x2, 0xDF, 0x20, 0x70, 0xC4, 0xE9, 0x1, 0x12, 0xE3, 0x60, 0x20, 0x46,
                        0x10, 0xBD, 0x30, 0xB5, 0x87, 0xB0, 0x0, 0x23, 0x2, 0xAC, 0x5, 0x46, 0x0, 0x93, 0x20, 0x46,
                        0x1A, 0x46, 0x2, 0x21, 0xFF, 0xF7, 0xE8, 0xFF, 0x0, 0x95, 0x94, 0xE8, 0x0F, 0x0, 0xFF, 0xF7,
                        0x65, 0xFF, 0x7, 0xB0, 0x30, 0xBD, 0x10, 0xB5, 0x86, 0xB0, 0x2, 0xAC, 0x0, 0x22, 0x3, 0x46,
                        0x0, 0x92, 0x20, 0x46, 0x1, 0x22, 0x2, 0x21, 0xFF, 0xF7, 0xD6, 0xFF, 0x94, 0xE8, 0x0F, 0x0,
                        0x6, 0xB0, 0xBD, 0xE8, 0x10, 0x40, 0xFF, 0xF7, 0x3B, 0xBF, 0x10, 0xB5, 0x86, 0xB0, 0x0, 0x22,
                        0x0, 0x92, 0x2, 0xAC, 0x2, 0x22, 0x3, 0x46, 0x11, 0x46, 0x20, 0x46, 0xFF, 0xF7, 0xC4, 0xFF,
                        0x94, 0xE8, 0x0F, 0x0, 0x6, 0xB0, 0xBD, 0xE8, 0x10, 0x40, 0xFF, 0xF7, 0x29, 0xBF, 0x10, 0xB5,
                        0x86, 0xB0, 0x0, 0x22, 0x2, 0xAC, 0x3, 0x46, 0x0, 0x91, 0x20, 0x46, 0x11, 0x46, 0xFF, 0xF7,
                        0x97, 0xFF, 0x94, 0xE8, 0x0F, 0x0, 0x6, 0xB0, 0xBD, 0xE8, 0x10, 0x40, 0xFF, 0xF7, 0x46, 0xBF,
                        0x10, 0xB5, 0x88, 0xB0, 0x4, 0xAC, 0x3, 0x46, 0x0, 0x91, 0x20, 0x46, 0x6, 0x22, 0x0, 0x21,
                        0xFF, 0xF7, 0xA2, 0xFF, 0x3, 0xAB, 0x0, 0x93, 0x94, 0xE8, 0x0F, 0x0, 0xFF, 0xF7, 0x1E, 0xFF,
                        0x8, 0xB0, 0x10, 0xBD, 0x30, 0xB5, 0x87, 0xB0, 0x0, 0x23, 0x2, 0xAC, 0x5, 0x46, 0x0, 0x93,
                        0x20, 0x46, 0x1, 0x22, 0x19, 0x46, 0xFF, 0xF7, 0x8F, 0xFF, 0x0, 0x95, 0x94, 0xE8, 0x0F, 0x0,
                        0xFF, 0xF7, 0x0C, 0xFF, 0x7, 0xB0, 0x30, 0xBD, 0x30, 0xB5, 0x87, 0xB0, 0x0, 0x23, 0x2, 0xAC,
                        0x5, 0x46, 0x0, 0x93, 0x20, 0x46, 0x2, 0x22, 0x19, 0x46, 0xFF, 0xF7, 0x7D, 0xFF, 0x0, 0x95,
                        0x94, 0xE8, 0x0F, 0x0, 0xFF, 0xF7, 0xFA, 0xFE, 0x7, 0xB0, 0x30, 0xBD, 0x8, 0xB5, 0x9, 0x4B,
                        0x4, 0x46, 0x59, 0xF8, 0x3, 0x30, 0x13, 0xB1, 0x0, 0x21, 0xAF, 0xF3, 0x0, 0x80, 0x6, 0x4B,
                        0x59, 0xF8, 0x3, 0x30, 0x18, 0x68, 0x83, 0x6A, 0x3, 0xB1, 0x98, 0x47, 0x20, 0x46, 0x0, 0xF0,
                        0x1B, 0xF8, 0x0, 0xBF, 0x1C, 0x0, 0x0, 0x0, 0x18, 0x0, 0x0, 0x0, 0x0A, 0x44, 0x91, 0x42,
                        0x0, 0xF1, 0xFF, 0x33, 0x0, 0xD1, 0x70, 0x47, 0x10, 0xB5, 0x11, 0xF8, 0x1, 0x4B, 0x91, 0x42,
                        0x3, 0xF8, 0x1, 0x4F, 0xF9, 0xD1, 0x10, 0xBD, 0x3, 0x46, 0x2, 0x44, 0x93, 0x42, 0x0, 0xD1,
                        0x70, 0x47, 0x3, 0xF8, 0x1, 0x1B, 0xF9, 0xE7, 0xFE, 0xE7, 0xFF, 0xFF, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x25, 0x1, 0x0, 0x80,
                        0x2D, 0x1, 0x0, 0x80, 0x9C, 0x8, 0x0, 0x0, 0xA0, 0x8, 0x0, 0x0, 0xA4, 0x8, 0x0, 0x0,
                        0xA8, 0x8, 0x0, 0x0, 0x90, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x98, 0x5, 0x0, 0x80,
                        0xB8, 0x5, 0x0, 0x80, 0x78, 0x5, 0x0, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2C, 0x8, 0x0, 0x0, 0x2C, 0x8, 0x0, 0x0,
                        0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x28, 0x0, 0x0, 0x0, 0x30, 0x8, 0x0, 0x0,
                        0x2, 0xAA, 0x0, 0x0, 0x34, 0x8, 0x0, 0x0, 0x2, 0xB5, 0x0, 0x0, 0x38, 0x8, 0x0, 0x0,
                        0x2, 0xA3, 0x0, 0x0, 0x8C, 0x8, 0x0, 0x0, 0x2, 0x3, 0x0, 0x0, 0x90, 0x8, 0x0, 0x0,
                        0x2, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
                        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

uint32_t crc32_poxit_lookup_table[256] = {0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9, 0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,
                                          0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61, 0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
                                          0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9, 0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
                                          0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011, 0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd,
                                          0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039, 0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,
                                          0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81, 0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
                                          0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49, 0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
                                          0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1, 0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,
                                          0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae, 0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
                                          0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16, 0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca,
                                          0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde, 0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02,
                                          0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066, 0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
                                          0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e, 0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692,
                                          0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6, 0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a,
                                          0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e, 0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
                                          0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686, 0xd5b88683, 0xd1799b34, 0xdc3abded, 0xd8fba05a,
                                          0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637, 0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
                                          0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f, 0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
                                          0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47, 0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,
                                          0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff, 0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623,
                                          0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7, 0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
                                          0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f, 0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
                                          0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7, 0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,
                                          0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f, 0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
                                          0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640, 0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,
                                          0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8, 0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24,
                                          0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30, 0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
                                          0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088, 0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654,
                                          0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0, 0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,
                                          0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18, 0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
                                          0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0, 0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c,
                                          0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668, 0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4};

// Callback Function
static void
ota_read_done(int length, __attribute__((unused)) int arg1, __attribute__((unused)) int arg2, __attribute__((unused)) void *ud)
{
  printf("\tFinished read! %i\n", length);
  done = true;
}

static void ota_write_done(int length, __attribute__((unused)) int arg1, __attribute__((unused)) int arg2, __attribute__((unused)) void *ud)
{
  printf("\tFinished write! %i\n", length);
  done = true;
}

static void button_callback(int btn_num, int val, int arg2 __attribute__((unused)), void *user_data __attribute__((unused)))
{
  // Callback for button presses.
  // btn_num: The index of the button associated with the callback
  // val: 1 if pressed, 0 if depressed
  if ((BUTTON_A == btn_num) && (BUTTON_PRESSED == val))
  {
    blink_binary_write();
  }
  else if ((BUTTON_B == btn_num) && (BUTTON_PRESSED == val))
  {
    // test_main();
  }
}

// Init function for setting up callback functions
int callback_subscribe_entry_initialzation(void)
{
  int ret;

  ret = ota_read_done_subscribe(ota_read_done, NULL);
  if (ret != RETURNCODE_SUCCESS)
  {
    printf("\tERROR setting read done callback\n");
    return ret;
  }

  ret = ota_write_done_subscribe(ota_write_done, NULL);
  if (ret != RETURNCODE_SUCCESS)
  {
    printf("\tERROR setting write done callback\n");
    return ret;
  }

  ret = button_subscribe(button_callback, NULL);
  if (ret != RETURNCODE_SUCCESS)
  {
    printf("\tERROR setting button callback\n");
    return ret;
  }

  return 0;
}

int button_init(void)
{
  int err;

  // Enable interrupts on each button.
  int count;
  err = button_count(&count);
  // Ensure there is a button to use.
  if (err < 0)
    return err;

  for (int i = 0; i < count; i++)
  {
    button_enable_interrupt(i);
  }

  return 0;
}

void blink_binary_write(void)
{
  int ret;
  uint32_t u32arry_len = sizeof(blkapp_bin) / sizeof(uint8_t);
  uint8_t readbuf[BUFFER_SIZE];
  uint8_t writebuf[BUFFER_SIZE];
  uint32_t u32start_address = 0;
  uint32_t u32FlashOffset = 0;
  uint32_t u32Processes_num = 0;
  uint32_t u32Crc32_kernel = 0;
  // uint8_t u8crc = 0xff;
  uint32_t u32crc = 0;

  // 0. Need to implement Permission process to access Nonvolatile_storate_driver and Process_load_utilities
  // To do..

  // 1. Get the number of processes
  ret = ota_get_the_number_of_processes(&u32Processes_num);
  if (ret < 0)
  {
    printf("Wirting the app into flash is impossible\n");
  }
  printf("\tProc_num at OTA: %ld\n", u32Processes_num);

  // 2. Find the start address of writable flash to install an application
  ret = ota_set_start_address_of_writable_flash();
  if (ret != 0)
  {
    printf("\tERROR calling read\n");
    // return ret;
  }

  // 3. Get the start address of writable flash to calculate the offset of nonvolatile_storage_drivers
  ret = ota_get_offset_of_start_address_of_writable_flash(&u32start_address);
  if (ret < 0)
  {
    printf("Wirting date into flash is impossible\n");
  }
  printf("\tstart address at OTA: %lx\n", u32start_address);

  // 4. Check the number of processes and the size of writable flash region. If this condition is not satisfied, skip writing application procedure
  if ((u32Processes_num < 4) && ((END_ADDRESS_OF_FLASH - u32start_address) > 0x800)) // TODO: Change hard coding..
  {
    ret = ota_internal_read_buffer(readbuf, BUFFER_SIZE);
    if (ret != RETURNCODE_SUCCESS)
    {
      printf("\tERROR setting read buffer\n");
      // return ret;
    }

    ret = ota_internal_write_buffer(writebuf, BUFFER_SIZE);
    if (ret != RETURNCODE_SUCCESS)
    {
      printf("\tERROR setting write buffer\n");
      // return ret;
    }

    // 5. write binary code into flash
    for (uint32_t u32offset = 0; u32offset < u32arry_len; u32offset += BUFFER_SIZE)
    {
      memcpy(writebuf, &blkapp_bin[0 + u32offset], BUFFER_SIZE);

      // 5.1 calculate crc during this loop
      /*for (uint16_t i = 0; i < BUFFER_SIZE; i++)
      {
        u8crc ^= writebuf[i];
        for (uint8_t j = 0; j < 8; j++)
        {
          if ((u8crc & 0x80) != 0)
          {
            u8crc = (uint8_t)((u8crc << 1) ^ CRC8_POLYNOMIAL);
          }
          else
          {
            u8crc <<= 1;
          }
        }
      }*/

      // 5.1 CRC32-POXIS (init: 0x00000000, poly: 0x04c11db7)
      for (uint16_t i = 0; i < BUFFER_SIZE; i++)
      {
        u32crc = (u32crc << 8) ^ crc32_poxit_lookup_table[((u32crc >> 24) ^ writebuf[i]) & 0xFF];
      }

      done = false;
      u32FlashOffset = u32offset + (u32start_address - START_ADDRESS_OF_APPLICATION);
      ret = ota_data_write_execution(u32FlashOffset, BUFFER_SIZE);
      if (ret != 0)
      {
        printf("\tERROR calling write\n");
        // return ret;
      }
      yield_for(&done);
    }
    // 5.2 Finalize
    u32crc ^= 0xffffffff;

    // 6. request to calculate crc32 and return the resulting value
    ret = ota_calculate_crc32(&u32Crc32_kernel);
    if (ret < 0)
    {
      printf("CRC32 calculation error\n");
    }
    printf("\tCRC32 Result at App layer: %ld\n", u32crc);
    printf("\tCRC32 Result at Kernel Layer: %ld\n", u32Crc32_kernel);

    // 7.Check CRC consistency between data sent and data flashed
    if (u32crc == u32Crc32_kernel)
    {
      ret = ota_app_loading_request();
      if (ret != 0)
      {
        printf("\tERROR calling app loading request\n");
      }
    }
    else
    {
      // Erase the falsh region to 0x01
      for (uint32_t u32offset = 0; u32offset < u32arry_len; u32offset += BUFFER_SIZE)
      {
        memset(writebuf, 0x01, BUFFER_SIZE); // set to value 1
        done = false;
        u32FlashOffset = u32offset + (u32start_address - START_ADDRESS_OF_APPLICATION);
        ret = ota_data_write_execution(u32FlashOffset, BUFFER_SIZE);
        if (ret != 0)
        {
          printf("\tERROR calling write\n");
        }
        yield_for(&done);
      }
    }

// This process will be replaced by CRC process
#if 0
    for (uint32_t u32offset = 0; u32offset < u32arry_len; u32offset += BUFFER_SIZE)
    {
      done = false;
      u32FlashOffset = u32offset + (u32start_address - START_ADDRESS_OF_APPLICATION);
      ret = ota_data_read_execution(u32offset, BUFFER_SIZE);
      if (ret != 0)
      {
        printf("\tERROR calling read\n");
        //  return ret;
      }
      yield_for(&done);

      for (int i = 0; i < BUFFER_SIZE; i++)
      {
        if (readbuf[i] != blkapp_bin[i + u32offset])
        {
          printf("\tInconsistency between data written and read at index %u\n", i);
          // return -1;
        }
      }
    }
#endif
  }
  else
  {
    printf("\tApplications cannot be updated more than 4\n");
    printf("\tFlash region is not enough to load the new application\n");
  }
}

int main(void)
{

  int ret;
  ret = callback_subscribe_entry_initialzation();
  if (ret != 0)
    return -1;

  ret = button_init();
  if (ret != 0)
    return -1;

  return 0;
}
