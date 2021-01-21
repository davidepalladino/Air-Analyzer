#include "Button.h"

Button::Button(uint8_t pin) : Button(pin, B_NOPULLUP, DEFAULT_LONG_PRESS, B_NOTCONTINUOUS, NULL, NULL) {}

Button::Button(uint8_t pin, input_t mode) : Button(pin, mode, DEFAULT_LONG_PRESS, B_NOTCONTINUOUS, NULL, NULL) {}

Button::Button(uint8_t pin, uint32_t timeLongPress) : Button(pin, B_NOPULLUP, timeLongPress, B_NOTCONTINUOUS, NULL, NULL) {}

Button::Button(uint8_t pin, uint32_t timeLongPress, longPress_t typeLongPress) : Button(pin, B_NOPULLUP, timeLongPress, typeLongPress, NULL, NULL) {}

Button::Button(uint8_t pin, input_t mode, uint32_t timeLongPress) : Button(pin, mode, timeLongPress, B_NOTCONTINUOUS, NULL, NULL) {}

Button::Button(uint8_t pin, input_t mode, uint32_t timeLongPress, longPress_t typeLongPress) : Button(pin, mode, timeLongPress, typeLongPress, NULL, NULL) {}

Button::Button(uint8_t pin, uint32_t timeLongPress, ptrProcedure ptrActionShort, ptrProcedure ptrActionLong) : Button(pin, B_NOPULLUP, timeLongPress, B_NOTCONTINUOUS, ptrActionShort, ptrActionLong) {}

Button::Button(uint8_t pin, uint32_t timeLongPress, longPress_t typeLongPress, ptrProcedure ptrActionShort, ptrProcedure ptrActionLong) : Button(pin, B_NOPULLUP, timeLongPress, typeLongPress, ptrActionShort, ptrActionLong) {}

Button::Button(uint8_t pin, input_t mode, uint32_t timeLongPress, ptrProcedure ptrActionShort, ptrProcedure ptrActionLong) : Button(pin, B_NOPULLUP, timeLongPress, B_NOTCONTINUOUS, ptrActionShort, ptrActionLong) {}

Button::Button(uint8_t pin, input_t mode, uint32_t timeLongPress, longPress_t typeLongPress, ptrProcedure ptrActionShort, ptrProcedure ptrActionLong) {
    setPin(pin);
    setMode(mode);

    setValuePress();

    pinMode(getPin(), getMode());
    
    setTimeLongPress(timeLongPress);
    setTypeLongPress(typeLongPress);

    setPtrActionShort(ptrActionShort);
    setPtrActionLong(ptrActionLong);
}

void Button::setTimeLongPress(uint32_t timeLongPress) { this->timeLongPress = timeLongPress; }

void Button::setTypeLongPress(longPress_t typeLongPress) { this->typeLongPress = typeLongPress; }

void Button::setPtrActionShort(ptrProcedure ptrActionShort) { this->ptrActionShort = ptrActionShort; }

void Button::setPtrActionLong(ptrProcedure ptrActionLong) { this->ptrActionLong = ptrActionLong; }

uint32_t Button::getTimeLongPress() { return this->timeLongPress; }

longPress_t Button::getTypeLongPress() { return this->typeLongPress; };

int8_t Button::checkPress() {   
    /* Read and save the value for next analysis. */
    uint8_t valueRead = digitalRead(getPin());

    /* Cheching if the button is pressed in this moment. */
    if (valueRead == getValuePress()) {
        /* Checking if is the first press or not. */
        if (!getIsPressed()) {
            //Serial.println("1.1");
            setIsPressed(true);

            /* Checking if is set the time of long press. If there is a value, will be set the timeout. */
            if (getTimeLongPress() > DEFAULT_LONG_PRESS) {
                setTimeOut(millis() + getTimeLongPress());
            }

        } else {
            /* Checking if is the long press. */
            if ((millis() >= getTimeOut()) && (getTimeLongPress() > DEFAULT_LONG_PRESS) && !isLongPressed) {
                //Serial.println("2.1");
                setActualValue(-1);
                setIsLongPressed(true);

                /* If there is a pointer to a procedure, will be executed. */
                if (this->ptrActionLong != NULL) {
                    this->ptrActionLong();
                    
                    while (valueRead == HIGH); 
                }
            } else if (isLongPressed && (getTypeLongPress() == B_NOTCONTINUOUS)) {
                //Serial.println("2.2");
                setActualValue(0);
            } else {
                //Serial.println("2.3");
                setActualValue(0);
            }
            
        }
    
    /* Else, the button is not pressed in this moment. */
    } else {
        /* Checking if is the short press, verifying if is set the "timeLongPress" or not. */      
        if (getIsPressed() && (((getActualValue() != -1) && (millis() < getTimeOut()) && !isLongPressed) || (getTimeLongPress() == DEFAULT_LONG_PRESS))) {
            //Serial.println("3.1");
            setActualValue(1);
            setIsPressed(false);
            setIsLongPressed(false);

            /* If there is a pointer to a procedure, will be executed. */
            if (this->ptrActionShort != NULL) {
                this->ptrActionShort();
            }

        /* Else, is not a press. */
        } else {
            //Serial.println("3.2");
            setActualValue(0);
            setIsPressed(false);
            setIsLongPressed(false);
        }
    }

    return getActualValue();
}

void Button::setPin(uint8_t pin) { this->pin = pin; }

void Button::setMode(input_t mode) {
    /* Translation of "mode" parameter "B_NOPULLUP"/"B_PULLUP" to the rispective "INPUT"/"INPUT_PULLUP". */
    if (mode == B_NOPULLUP) {
        this->mode = INPUT;
    } else if (mode == B_PULLUP) {
        this->mode = INPUT_PULLUP;
    }
}

void Button::setValuePress() {
    /* Checking what is the value of press, "HIGH" if the mode is INPUT; "LOW" if is INPUT_PULLUP. */
    if (getMode() == INPUT) {
        valuePress = HIGH;
    } else if (getMode() == INPUT_PULLUP) {
        valuePress = LOW;
    }
}

void Button::setTimeOut(uint32_t timeOut) { this->timeOut = timeOut; }

void Button::setIsPressed(bool isPressed) { this->isPressed = isPressed; }

void Button::setIsLongPressed(bool isLongPressed) { this->isLongPressed = isLongPressed; }

void Button::setActualValue(int8_t actualValue) { this->actualValue = actualValue; }

uint8_t Button::getPin() { return this->pin; }

uint8_t Button::getMode() { return this->mode; }

uint8_t Button::getValuePress() { return this->valuePress; }

uint32_t Button::getTimeOut() { return this->timeOut; }

bool Button::getIsPressed() { return this->isPressed; }

bool Button::getIsLongPressed() { return this->isLongPressed; }  

int8_t Button::getActualValue() { return this->actualValue; }