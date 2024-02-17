#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <timer.h>

#include <tock.h>
#include <app_loader.h>

/* Buffer Size */
#define FLASH_BUFFER_SIZE 512
#define RETURNCODE_SUCCESS 0

static bool done = false;     // to check if writing to flash is done
uint32_t app_size = 0;        // variable to store app size

// Tock Application Binary to be flashed.
const uint8_t app_binary[] = {0x2, 0x0, 0x34, 0x0, 0x0, 0x8, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0xD7, 0x75, 0x50, 0x6E,
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


/******************************************************************************************************
 * 
 * Function to initialize buttons and enable their interrupts
 * 
 ******************************************************************************************************/

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


/******************************************************************************************************
 * Callback functions
 * 
 * 1. Callback to let us know when the capsule is done writing data to flash
 * 2. Set button callback to initiate the dynamic app load process on pressing button 1 (on nrf52840dk)
 * 
 ******************************************************************************************************/

// static void nop_callback(int a __attribute__((unused)), int b __attribute__((unused)), int c __attribute__((unused)), void *d __attribute__((unused))) {}

static void app_write_done_callback(__attribute__((unused)) int length, __attribute__((unused)) int arg1, __attribute__((unused)) int arg2, __attribute__((unused)) void *ud)
{
  // printf("Finished section write! %i\n", length);
  done = true;
}

static void button_callback(int btn_num, int val, int arg2 __attribute__((unused)), void *user_data __attribute__((unused)))
{
  // Callback for button presses.
  // btn_num: The index of the button associated with the callback
  // val: 1 if pressed, 0 if depressed
  if ((btn_num == BUTTON_1) && (val == BUTTON_PRESSED))
  {
    int ret;
    app_size = sizeof(app_binary);
    ret = app_loader_command_setup(app_size);     //asks the capsule to set up for app flash
    if (ret != RETURNCODE_SUCCESS){
      printf("[Error] Setup Failed.\n");
    }

    int ret1;
    ret1 = write_app();                           // writes app data to flash
    if (ret1 != RETURNCODE_SUCCESS){
      printf("[Error] App Flash Write unsuccessful.");
    }

    int ret2;
    ret2 = app_loader_command_load();             // request to load app
    if (ret2 != RETURNCODE_SUCCESS){
      printf("[Error] App Load Failed.\n");
    }
  }
}


/******************************************************************************************************
 * 
 * Function to write the app into the flash
 * 
 ******************************************************************************************************/

int write_app(void){

  int ret;
  uint32_t write_count = 0;
  uint8_t write_buffer[FLASH_BUFFER_SIZE];
  uint32_t flash_offset = 0;
  
  write_count = sizeof(app_binary) / FLASH_BUFFER_SIZE;

  ret = app_loader_write_buffer(write_buffer, FLASH_BUFFER_SIZE);
  if (ret != RETURNCODE_SUCCESS)
  {
    printf("[Error] Failed to set the write buffer.\n");
    return -1;
  }

  for (uint32_t offset = 0; offset < write_count; offset++)
    { 
      memcpy(write_buffer, &app_binary[FLASH_BUFFER_SIZE * offset], FLASH_BUFFER_SIZE);   // copy binary to write buffer
      done = false;
      flash_offset = (offset * FLASH_BUFFER_SIZE);
      ret = app_loader_command_write(flash_offset, FLASH_BUFFER_SIZE);
      if (ret != 0)
      {
        printf("[Error] Failed writing data to flash at address: 0x%lx\n", flash_offset);
        return -1;
      }
      yield_for(&done);   // wait until write done callback
    }
  return 0;
}


/******************************************************************************************************
 * 
 * Function to set up the write done and button press callbacks
 * 
 ******************************************************************************************************/

int app_loader_callback_subscribe_init(void)
{
  int ret_app_subs = app_loader_subscribe(app_write_done_callback, NULL);
  if (ret_app_subs != RETURNCODE_SUCCESS)
  {
    printf("[Error] Failed to set flash write done callback.\n");
    return ret_app_subs;
  }

  int ret_btn_subs = button_subscribe(button_callback, NULL);
  if (ret_btn_subs != RETURNCODE_SUCCESS)
  {
    printf("[Error] Failed to set button callback.\n");
    return ret_btn_subs;
  }

  return 0;
}


/******************************************************************************************************
 * 
 * Main
 * 
 ******************************************************************************************************/

int main(void) {
  printf("Simple test app to flash and load an app dynamically.\n");

  volatile int ret = 0;

  ret = button_init();
  if (ret != 0)
  {
    printf("[Error] Button init failed: %d\n", ret);
    return -1;
  }

  ret = app_loader_callback_subscribe_init();
  if (ret != 0)
      printf("[Error] Apploader callback subscribe init failed: %d\n", ret);
    return -1;

  return 0;
} 
