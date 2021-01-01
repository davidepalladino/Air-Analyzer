#include <Screen.h>

Screen::Screen(Sensor& sensor, uint8_t pinSCL, uint8_t pinSDA) : sensor(sensor) {
    this->sensor.addObserver(this);
    this->screen = new U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C (U8G2_R0, pinSCL, pinSDA);
}

void Screen::begin() {
    this->screen->begin();
}

void Screen::drawIcons() {
    this->screen->drawXBMP(positionLogoTemperature[0], positionLogoTemperature[1], logoTemperatureWidth, logoTemperatureHeight, logoTemperature);
    this->screen->drawXBMP(positionLogoHumidity[0], positionLogoHumidity[1], logoHumidityWidth, logoHumidityHeight, logoHumidity);
}

void Screen::drawValues() {
    this->screen->setFont(u8g2_font_smart_patrol_nbp_tf);
    this->screen->setCursor(positionValueTemperature[0], positionValueTemperature[1]);
    this->screen->print(sensor.getTemperature());
    this->screen->setCursor(positionValueHumidity[0], positionValueHumidity[1]);
    this->screen->print(String(sensor.getHumidity()));
}

void Screen::drawUnits() {
    this->screen->setCursor(positionUnitTemperature[0], positionUnitTemperature[1]);
    this->screen->print(String((char) 176) + "C");
    this->screen->setCursor(positionUnitHumidity[0], positionUnitHumidity[1]);
    this->screen->print(" %");
}

void Screen::update() {
    this->screen->firstPage();
    do {
        drawIcons();
        drawValues();
        drawUnits();
    } while (this->screen->nextPage());
}