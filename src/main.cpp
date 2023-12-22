#include <mbed.h>
#include "OledSSD1306.h"

#define OLED_I2C_SDA PB_9
#define OLED_I2C_SCL PB_8

int main() {

    printf("Running on Mbed OS %d.%d.%d \n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    I2C i2c(OLED_I2C_SDA, OLED_I2C_SCL);

    for (int i = 0; i < 0xFF; ++i) {
        int ack = i2c.write(i, 0, 0);
        if (!ack) printf("0x%x has a slave\n", i);
        else printf("0x%x No slave\n", i);
    }

    OledSSD1306 oled(i2c, SA0);

    oled.start(INTERNAL_VCC);
    oled.setFrequency(400000);
    oled.invert(true);
    oled.drawPixel(15, 15, 1);
    oled.drawPixel(16, 15, 1);
    oled.sendDisplayBuffer();
}
