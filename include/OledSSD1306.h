#ifndef OLEDSSD1306_H
#define OLEDSSD1306_H

#include <mbed.h>

#include "fonts/font_8x8.h"

#define charc static_cast<char>

// I2C implementation
// SA0 = LOW (grounded)
// SA1 = HIGH
#define SA0 (0x3C << 1) // 0b0111100
#define SA1 (0x3D << 1)// 0b0111101

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#define BUFF_SIZE (DISPLAY_WIDTH * ((DISPLAY_HEIGHT + 7) / 8))

#define PAGES (HEIGHT / 8)
#define PAGE_START_ADDRESS 0xB0

// Modes
#define COMMAND_MODE 0x00
#define DATA_MODE 0x40

#define CONTRAST_COMMAND 0x81
// Entire display on / Output ignores RAM content
#define DISPLAY_ON_ALL_COMMAND 0xA5
// Resume to RAM content display
#define DISPLAY_ON_ALL_RAM_COMMAND 0xA4

#define NORMAL_DISPLAY_COMMAND 0xA6
#define INVERSE_DISPLAY_COMMAND 0xA7

// Set display in sleep mode
#define DISPLAY_OFF_COMMAND 0xAE
#define DISPLAY_ON_NORMAL_COMMAND 0xAF

// Scrolling commands
#define RIGHT_HORIZONTAL_SCROLL 0x26
#define LEFT_HORIZONTAL_SCROLL 0x27
#define VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A
#define DEACTIVATE_SCROLL 0x2E
#define ACTIVATE_SCROLL 0x2F
#define SET_VERTICAL_SCROLL_AREA 0xA3

// Addressing setting
#define MEMORY_ADDRESSING_MODE 0x20
#define HIGHER_COLUMN_COMMAND 0x10

// Hardware configuration

// Display start line from 0-63 starting from 000000b to 111111b
#define DISPLAY_START_LINE_COMMAND 0x40
#define MULTIPLEX_RATIO_COMMAND 0xA8
#define DISPLAY_OFFSET_COMMAND 0xD3

#define DISPLAY_CLOCK_COMMAND 0xD5
#define DISPLAY_CHARGE_PUMP_COMMAND 0x8D

#define EXTERNAL_VCC 0x01  // external voltage
#define INTERNAL_VCC 0x02 // internal voltage 3.3V or 5V

#define SEGMENT_REMAP 0xA0 // 0x0 = column addr 0 mapped to SEG0, 0x1 = column addr 127 mapped to SEG0
#define SCAN_DIRECTION 0xC8 // remapped mode. scan from COM(N-1) to COm0 (N = multiplex ratio)

#define SET_COM_PINS 0xDA

#define PRE_CHARGE_PERIOD 0xD9

#define BLACK_COLOR 0x00
#define WHITE_COLOR 0x01
#define INVERSE_COLOR 0x02

#define VCOMDETECT 0xDB

struct Cursor {
    uint8_t x;
    uint8_t y;
};

class OledSSD1306 {

private:
    I2C &m_i2c;
    Thread m_thread;
    std::array<uint8_t, BUFF_SIZE> m_buff{};

    uint8_t m_i2cAddress{};
    uint8_t m_vcc{};

    bool m_inverted = false;

    Cursor m_cursor{0, 0};

    uint8_t* m_font = nullptr;

    void sendCommand(uint8_t command) const;
    void sendCommand(uint8_t command1, uint8_t command2) const;
    void sendData(uint8_t data) const;
    void sendData(uint8_t data, uint8_t length) const;

    void runUpdateThread() const;

public:
    explicit OledSSD1306(I2C &i2c);
    explicit OledSSD1306(I2C &i2c, uint8_t i2cSlaveAddress = SA0);
    ~OledSSD1306();

    void start(uint8_t vccState);

    void clear();
    void invert();
    void fill();

    void drawPixel(uint16_t x, uint16_t y, uint16_t color);

    void print(unsigned char text, uint16_t color);

    void setFont(uint8_t* font);

    void setCursor(uint16_t x, uint16_t y) {
        m_cursor.x = x;
        m_cursor.y = y;
    };

    void setContrast(uint8_t value) const;

    void setFrequency(const int frequency) const {
        m_i2c.frequency(frequency);
    }

    bool getPixel(uint16_t x, uint16_t y) const;

    void sendDisplayBuffer() const;
};

#endif //OLEDSSD1306_H
