#include <iostream>
#include <mbed.h>
#include "OledSSD1306.h"

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

int main() {

    printf("Running on Mbed OS %d.%d.%d \n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

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
    //oled.setFont(font8x8_basic);
    //oled.print("Test", 1);
    oled.fill();
    ThisThread::sleep_for(5s);
    oled.clear();

    oled.setFont(ArialMT_16);
    oled.drawString(0, 0, "Hello world", 1);

    oled.sendDisplayBuffer();
}
