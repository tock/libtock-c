#include <button.h>
#include <led.h>
#include <spi.h>
#include <console.h>

/*
 * This program waits for the USER button to be pressed,
 * and then reads a one-character instruction from the console.
 *
 * If it is 'b', the program begins sending data on the SPI bus.
 * If it is 'e', the program stops sending data.
 */

static void button_callback(int, int, int, void *);
static void spi_initialize(void);
static void spi_begin(void);
static void spi_end(void);
static void spi_continue(void);
static void spi_written(int, int, int, void *);

int main(void) {
    // Setup SPI for later use
    spi_initialize();

    // Setup button callback
    button_subscribe(button_callback, NULL);

    // Wait for a button press
    button_enable_interrupt(0);

    // Signal that the app is waiting for input
    led_off(0);

    printf("(Press user button before issuing each instruction.)\n");
}

static void button_callback(int btn_num,
                            int val,
                            __attribute__ ((unused)) int arg2,
                            __attribute__ ((unused)) void *ud) {

    if (btn_num == 0 && val == 1) {
        // Button 0 was pressed

        // Ignore button presses
        button_disable_interrupt(0);

        // Wait for instruction from console
        led_on(0);
        int c = getch();
        led_off(0);

        // Handle the instruction
        switch (c) {
            case TOCK_FAIL:
                printf("getch() failed!\n");
                break;
            case 'b':
                spi_begin();
                break;
            case 'e':
                spi_end();
                break;
            default:
                printf("Ignore '%c'\n", c);
                break;
        }

        // Wait for the next button press
        button_enable_interrupt(0);
    }
}

#define SPI_BUF_SIZE 200
static char wbuf[SPI_BUF_SIZE];
static bool spi_running;
static int spi_write_count;

static void spi_initialize(void) {
    int i;
    for (i = 0; i < 200; i++) {
      wbuf[i] = i;
    }
    spi_init();
    spi_set_chip_select(4);
    spi_set_rate(400000);
    spi_set_polarity(false);
    spi_set_phase(false);

    spi_running = false;
}

static void spi_begin(void) {
    if (spi_running) {
        printf("(SPI already running)\n");
        return;
    }

    printf("SPI Beginning\n");
    spi_write_count = 0;
    spi_running = true;
    spi_continue();
}

static void spi_end(void) {
    if (!spi_running) {
        printf("(SPI not running)\n");
        return;
    }

    printf("SPI Ending (after %d writes)\n", spi_write_count);
    spi_running = false;
}

static void spi_continue(void) {
    spi_write(wbuf, SPI_BUF_SIZE, spi_written, NULL);
}

static void spi_written(__attribute__ ((unused)) int arg0,
                        __attribute__ ((unused)) int arg2,
                        __attribute__ ((unused)) int arg3,
                        __attribute__ ((unused)) void* userdata) {
    spi_write_count++;

    if (spi_running) {
        // Do another write
        spi_continue();
    }
}
