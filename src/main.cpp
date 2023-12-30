#include <iomanip>
#include <iostream>
#include <mbed.h>
#include "OledSSD1306.h"
#include "fonts/font_8x8.h"

#define OLED_I2C_SDA PB_9
#define OLED_I2C_SCL PB_8

void checkI2CAddr(I2C &i2c) {
    for (int i = 0; i < 0xFF; ++i) {
        const int ack = i2c.write(i, nullptr, 0);
        if (!ack) printf("0x%x has a slave\n", i);
        else printf("0x%x No slave\n", i);
    }
}

void invertDisplay(OledSSD1306 &display, Timer &debouce) {
    if (chrono::duration_cast<chrono::milliseconds>(debouce.elapsed_time()).count() <= 10) return;
    display.invert();
    debouce.reset();
}

void printBuffer(array<uint8_t, BUFF_SIZE> &buff) {
}

int main() {

    printf("Running on Mbed OS %d.%d.%d \n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            printf("%s", font8x8_basic[i]);
        }
    }
    printf("\n");

    I2C i2c(OLED_I2C_SDA, OLED_I2C_SCL);
    OledSSD1306 oled(i2c, SA0);

    //Timer debounce;
    //debounce.start();

    //InterruptIn invertButton(PB_0);
    //invertButton.mode(PullUp);
    //invertButton.rise([&oled, &debounce] { invertDisplay(oled, debounce);});
    //invertButton.enable_irq();

    oled.start(INTERNAL_VCC);
    //oled.invert();
    //for (int i = 0; i < DISPLAY_WIDTH; ++i) {
    //    for (int j = 0; j < DISPLAY_HEIGHT; ++j) {
    //        oled.drawPixel(i, j, 1);
    //    }
    //}
    //oled.fill();

    //oled.drawPixel(15, 15, 1);
    //oled.drawPixel(16, 15, 1);

    oled.print('A', 1);

    oled.sendDisplayBuffer();


}
