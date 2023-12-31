#include <mbed.h>
#include "../include/OledSSD1306.h"

#include <iomanip>
#include <iostream>

OledSSD1306::OledSSD1306(I2C& i2c) : m_i2c(i2c), m_i2cAddress(0x3C) {}

OledSSD1306::OledSSD1306(I2C& i2c, uint8_t i2cSlaveAddress) : m_i2c(i2c), m_i2cAddress(i2cSlaveAddress) {
    //this->m_thread.start(callback(this, &OledSSD1306::runUpdateThread));
}

OledSSD1306::~OledSSD1306() {
    //m_thread.join();

}

void OledSSD1306::runUpdateThread() const{
    //while (true) {
    //    sendDisplayBuffer();
    //}
}

void OledSSD1306::start(uint8_t vccState) {
    clear();
    m_vcc = vccState;

    sendCommand(DISPLAY_OFF_COMMAND);
    sendCommand(DISPLAY_CLOCK_COMMAND, 0x80);

    //sendCommand(MULTIPLEX_RATIO_COMMAND, 0x3F);
    sendCommand(MULTIPLEX_RATIO_COMMAND);
    sendCommand(0x3F); // HEIGHT - 1

    sendCommand(DISPLAY_OFFSET_COMMAND, 0x00);
    sendCommand(DISPLAY_START_LINE_COMMAND | 0x00);

    //sendCommand(DISPLAY_CHARGE_PUMP_COMMAND, 0x14);
    sendCommand(DISPLAY_CHARGE_PUMP_COMMAND);

    sendCommand((vccState == INTERNAL_VCC) ? 0x14 : 0x10);

    sendCommand(MEMORY_ADDRESSING_MODE, 0x00); // horizontal addressing
    sendCommand(SEGMENT_REMAP, 0x01);
    sendCommand(SCAN_DIRECTION);

    // 128x64
    sendCommand(SET_COM_PINS, 0x12);
    //sendCommand(CONTRAST_COMMAND, (vccState == INTERNAL_VCC) ? 0xCF : 0x9F);
    sendCommand(CONTRAST_COMMAND, 0xFF);

    sendCommand(PRE_CHARGE_PERIOD, (vccState == INTERNAL_VCC) ? 0xF1 : 0x22);

    sendCommand(VCOMDETECT, 0x40);

    sendCommand(DISPLAY_ON_ALL_RAM_COMMAND);
    sendCommand(NORMAL_DISPLAY_COMMAND);
    sendCommand(DEACTIVATE_SCROLL);
    sendCommand(DISPLAY_ON_NORMAL_COMMAND);
    clear();
}

void OledSSD1306::sendCommand(uint8_t command) const {
    const char buff[] = { COMMAND_MODE, charc (command) };
    m_i2c.write(m_i2cAddress, buff, sizeof(buff));
}

void OledSSD1306::sendCommand(uint8_t command1, uint8_t command2) const {
    const char buff[] = { COMMAND_MODE, charc (command1), COMMAND_MODE, charc (command2) };
    m_i2c.write(m_i2cAddress, buff, sizeof(buff));
}

void OledSSD1306::sendData(uint8_t data) const {
    const char buff[] = { DATA_MODE, charc (data)};
    m_i2c.write(m_i2cAddress, buff, sizeof(buff));
}

void OledSSD1306::sendData(uint8_t data, uint8_t length) const {
    const char buff[] = { DATA_MODE, charc (data)};
    m_i2c.write(m_i2cAddress, buff, length);
}

void OledSSD1306::clear() {
    // TODO: Scrolling set page Offset 0 & Start line 0
    //fill(begin(m_buff), end(m_buff), 0);
    m_buff.fill(0);
    //sendDisplayBuffer();
}

void OledSSD1306::invert() {
    sendCommand(!m_inverted ? INVERSE_DISPLAY_COMMAND : NORMAL_DISPLAY_COMMAND);
    m_inverted = !m_inverted;
}

void OledSSD1306::fill() {
    std::fill(begin(m_buff), end(m_buff), 0xFF);
}

void OledSSD1306::setContrast(uint8_t value) const {
    sendCommand(CONTRAST_COMMAND, value);
}

void OledSSD1306::drawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if (x > DISPLAY_WIDTH || y > DISPLAY_WIDTH) return;

    /*
     * Calculate the height position and add the x value
     * Take the rest of the modulo and shift the 1 to this position
    */
    switch (color) {
        case WHITE_COLOR:
            m_buff[DISPLAY_WIDTH * (y / 8) + x] |= 1 << (y % 7);
            break;
        case BLACK_COLOR:
            m_buff[DISPLAY_WIDTH * (y / 8) + x] &= 1 << (y % 8);
            break;
        case INVERSE_COLOR:
            m_buff[DISPLAY_WIDTH * (y / 8) + x] ^= 1 << (y % 8);
            break;
        default:
            error("Colors can only be 0 = White, 1 = Black or 2 = Inversed. View header file for macros");
    }
    sendDisplayBuffer();
}

void OledSSD1306::print(unsigned char c, uint16_t color) {
    const uint8_t* font = font8x8_basic[(uint8_t) c];
    for (int i = 0; i < 8; i++) {
        uint8_t line = (*(const unsigned char *)(&font[(uint8_t) c * 8 + i]));
        for (int j = 0; j < 8; j++, line >>= 1) {
            drawPixel(0 + i, 0 + j, color);
        }
    }
}


void OledSSD1306::sendDisplayBuffer() const {

    //cout << "Array content: [ ";
    //for (const auto& element : m_buff) {
    //    // Print each element as a two-digit hexadecimal number
    //    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(element) << " ";
    //}
    //std::cout << "]" << std::endl;

    for (const uint8_t pixel : m_buff) {
        sendData(pixel);
    }
}

void OledSSD1306::setFont(uint8_t* font) {
    m_font = font;
}

bool OledSSD1306::getPixel(uint16_t x, uint16_t y) const {
    if (x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT) {
        return (m_buff[DISPLAY_WIDTH * (y / 8) + x] & (1 << (y & 7)));
    }
    return false;
}