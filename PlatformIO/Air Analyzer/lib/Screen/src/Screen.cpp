#include <Screen.h>

Screen::Screen(Sensor& sensor, uint8_t pinSCL, uint8_t pinSDA) : sensor(sensor) {
    this->sensor.addObserver(this);
    this->screen = new U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C (U8G2_R0, pinSCL, pinSDA);

    roomID = 0;
    isConnectedWiFi = true;
    isErrorUpdate = false;
}

void Screen::begin() { this->screen->begin(); }

void Screen::setRoomID(uint8_t roomID) { this->roomID = roomID; }

void Screen::setIsConnectedWiFi(bool isConnectedWiFi) { this->isConnectedWiFi = isConnectedWiFi; }

bool Screen::getIsConnectedWiFi() { return this->isConnectedWiFi; }

void Screen::setIsErrorUpdate(bool isErrorUpdate) { this->isErrorUpdate = isErrorUpdate; }

bool Screen::getIsErrorUpdate() { return this->isErrorUpdate; }

void Screen::showLoadingPage(String message, float percentage) {
    this->screen->firstPage();
    do {
        drawBar(percentage);
        drawMessage(positionMessageLoadingPage[0], positionMessageLoadingPage[1], message);
    } while (this->screen->nextPage());
}

void Screen::showMainPage() {
    this->screen->firstPage();
    do {
        drawIcons();
        drawValues();
        drawUnits();
    } while (this->screen->nextPage());
}

void Screen::showMessagePage(String message) {
    this->screen->firstPage();
    do {
        drawMessage(positionMessageMessagePage[0], positionMessageMessagePage[1], message);
    } while (this->screen->nextPage());
}

void Screen::showMessagePage(String messageOne, String messageTwo) {
    this->screen->firstPage();
    do {
        drawMessage(positionMessageMessagePage[0], positionMessageMessagePage[1] - 6, messageOne);
        drawMessage(positionMessageMessagePage[0], positionMessageMessagePage[1] + 6, messageTwo);
    } while (this->screen->nextPage());
}

void Screen::drawBar(float percentage) {
    uint8_t widthFill = map(percentage, 0, 100, 0, sizeFrameLoadingPage[0]);
    this->screen->drawFrame(positionFrameLoadingPage[0], positionFrameLoadingPage[1], sizeFrameLoadingPage[0], sizeFrameLoadingPage[1]);
    this->screen->drawBox(positionFrameLoadingPage[0], positionFrameLoadingPage[1], widthFill, sizeFrameLoadingPage[1]);
}

void Screen::drawMessage(uint8_t positionX, uint8_t positionY, String message) {
    this->screen->setFont(u8g2_font_profont11_mf);
    this->screen->drawStr(positionX, positionY, message.c_str());
}

void Screen::drawIcons() {
    this->screen->drawXBMP(positionLogoTemperature[0], positionLogoTemperature[1], logoTemperatureWidth, logoTemperatureHeight, logoTemperature);
    this->screen->drawXBMP(positionLogoHumidity[0], positionLogoHumidity[1], logoHumidityWidth, logoHumidityHeight, logoHumidity);
    this->screen->drawCircle(positionCircleRoomID[0], positionCircleRoomID[1], radiusCircleRoomID, U8G2_DRAW_ALL);
    if (isConnectedWiFi) {
        this->screen->setFont(u8g2_font_open_iconic_www_1x_t);
        this->screen->drawGlyph(positionLogoWiFi[0], positionLogoWiFi[1], 0x0048); 
    }

    if (isErrorUpdate) {
        this->screen->setFont(u8g2_font_open_iconic_embedded_1x_t);
        this->screen->drawGlyph(positionLogoError[0], positionLogoError[1], 0x0047); 
    }
}

void Screen::drawValues() {
    this->screen->setFont(u8g2_font_smart_patrol_nbp_tf);
    this->screen->setCursor(positionValueTemperature[0], positionValueTemperature[1]);
    this->screen->print(sensor.getTemperature(), 1);
    this->screen->setCursor(positionValueHumidity[0], positionValueHumidity[1]);
    this->screen->print(sensor.getHumidity(), 1);

    if (roomID == 1) {
        this->screen->setCursor(positionValueRoomID[0], positionValueRoomID[2]);
    } else {
        this->screen->setCursor(positionValueRoomID[1], positionValueRoomID[2]);
    }
    this->screen->print(this->roomID); 
}

void Screen::drawUnits() {
    this->screen->setFont(u8g2_font_smart_patrol_nbp_tf);
    this->screen->setCursor(positionUnitTemperature[0], positionUnitTemperature[1]);
    this->screen->print(String((char) 176) + "C");
    this->screen->setCursor(positionUnitHumidity[0], positionUnitHumidity[1]);
    this->screen->print(" %");
}

void Screen::update() {
    showMainPage();    
}