#include <Screen.h>

Screen::Screen(Sensor& sensor, uint8_t pinSCL, uint8_t pinSDA) : sensor(sensor) {
    sensor.addObserver(this);
    screen = new U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C (U8G2_R0, pinSCL, pinSDA);

    roomID = 0;

    isConnectedWiFi = true;
    isErrorUpdate = false;
}

void Screen::begin() { screen->begin(); }

void Screen::setRoomID(uint8_t roomID) { this->roomID = roomID; }

void Screen::setIsConnectedWiFi(bool isConnectedWiFi) { this->isConnectedWiFi = isConnectedWiFi; }

bool Screen::getIsConnectedWiFi() { return isConnectedWiFi; }

void Screen::setIsErrorUpdate(bool isErrorUpdate) { this->isErrorUpdate = isErrorUpdate; }

bool Screen::getIsErrorUpdate() { return isErrorUpdate; }

void Screen::showInstallationRoomIDPage(const String messages[3]) {
    screen->firstPage();
    do {
        drawRoomID();
        drawMessage(positionMessageInstallationRoomIDPage[0], positionMessageInstallationRoomIDPage[1], messages[0]);
        drawMessage(positionMessageInstallationRoomIDPage[0], positionMessageInstallationRoomIDPage[2], messages[1]);
        drawMessage(positionMessageInstallationRoomIDPage[0], positionMessageInstallationRoomIDPage[3], messages[2]);
    } while (screen->nextPage());
}

void Screen::showInstallationWiFiPage(const String messages[6], uint8_t result) {
    screen->firstPage();
    do {
        drawWiFiStatus();
        switch (result) {
            case 0:
                drawMessage(positionMessageInstallationWiFiPageCase0[0], positionMessageInstallationWiFiPageCase0[1], messages[0]);
                drawMessage(positionMessageInstallationWiFiPageCase0[0], positionMessageInstallationWiFiPageCase0[2], messages[1]);
                drawMessage(positionMessageInstallationWiFiPageCase0[0], positionMessageInstallationWiFiPageCase0[3], messages[2]);
                break;
            case 1:
                drawMessage(positionMessageInstallationWiFiPageCase1[0], positionMessageInstallationWiFiPageCase1[1], messages[3]);
                drawMessage(positionMessageInstallationWiFiPageCase1[0], positionMessageInstallationWiFiPageCase1[2], messages[4]);
                break;
            case 2:
                drawMessage(positionMessageInstallationWiFiPageCase2[0], positionMessageInstallationWiFiPageCase2[1], messages[5]);
                break;
        }
    } while (screen->nextPage());
}

void Screen::showLoadingPage(String message, float percentage) {
    screen->firstPage();
    do {
        drawBar(percentage);
        drawMessage(positionMessageLoadingPage[0], positionMessageLoadingPage[1], message);
    } while (screen->nextPage());
}

void Screen::showMainPage() {
    screen->firstPage();
    do {
        drawRoomID();
        drawTemperature();
        drawHumidity();
        drawWiFiStatus();
        drawErrorStatus();
    } while (screen->nextPage());
}

void Screen::showMessagePage(String message) {
    screen->firstPage();
    do {
        drawMessage(positionMessageMessagePage[0], positionMessageMessagePage[1], message);
    } while (screen->nextPage());
}

void Screen::showMessagePage(const String messages[2]) {
    screen->firstPage();
    do {
        drawMessage(positionMessageMessagePage[0], positionMessageMessagePage[1] - 6, messages[0]);
        drawMessage(positionMessageMessagePage[0], positionMessageMessagePage[1] + 6, messages[1]);
    } while (screen->nextPage());
}

void Screen::drawBar(float percentage) {
    uint8_t widthFill = map(percentage, 0, 100, 0, sizeFrameLoadingPage[0]);
    screen->drawFrame(positionFrameLoadingPage[0], positionFrameLoadingPage[1], sizeFrameLoadingPage[0], sizeFrameLoadingPage[1]);
    screen->drawBox(positionFrameLoadingPage[0], positionFrameLoadingPage[1], widthFill, sizeFrameLoadingPage[1]);
}

void Screen::drawMessage(uint8_t positionX, uint8_t positionY, String message) {
    screen->setFont(u8g2_font_profont11_mf);
    screen->drawStr(positionX, positionY, message.c_str());
}

void Screen::drawRoomID() {
    screen->drawCircle(positionCircleRoomID[0], positionCircleRoomID[1], radiusCircleRoomID, U8G2_DRAW_ALL);
    screen->setFont(u8g2_font_smart_patrol_nbp_tf);
    if (roomID == 1) {
        screen->setCursor(positionValueRoomID[0], positionValueRoomID[2]);
    } else {
        screen->setCursor(positionValueRoomID[1], positionValueRoomID[2]);
    }
    screen->print(roomID); 
}

void Screen::drawTemperature() {
    screen->drawXBMP(positionLogoTemperature[0], positionLogoTemperature[1], logoTemperatureWidth, logoTemperatureHeight, logoTemperature);
    screen->setFont(u8g2_font_smart_patrol_nbp_tf);
    screen->setCursor(positionValueTemperature[0], positionValueTemperature[1]);
    screen->print(sensor.getTemperature(), 1);
    screen->setCursor(positionUnitTemperature[0], positionUnitTemperature[1]);
    screen->print(String((char) 176) + "C");
}

void Screen::drawHumidity() {
    screen->drawXBMP(positionLogoHumidity[0], positionLogoHumidity[1], logoHumidityWidth, logoHumidityHeight, logoHumidity);
    screen->setFont(u8g2_font_smart_patrol_nbp_tf);
    screen->setCursor(positionValueHumidity[0], positionValueHumidity[1]);
    screen->print(sensor.getHumidity(), 1);
    screen->setCursor(positionUnitHumidity[0], positionUnitHumidity[1]);
    screen->print(" %");   
}

void Screen::drawWiFiStatus() {
    if (isConnectedWiFi) {
        screen->setFont(u8g2_font_open_iconic_www_1x_t);
        screen->drawGlyph(positionLogoWiFi[0], positionLogoWiFi[1], 0x0048); 
    }
}

void Screen::drawErrorStatus() {
    if (isErrorUpdate) {
        screen->setFont(u8g2_font_open_iconic_embedded_1x_t);
        screen->drawGlyph(positionLogoError[0], positionLogoError[1], 0x0047); 
    }
}

void Screen::update() {
    showMainPage();    
}