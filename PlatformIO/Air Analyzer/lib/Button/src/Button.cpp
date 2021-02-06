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
    this->pin = pin;
    setMode(mode);

    setValuePress();

    pinMode(this->pin, this->mode);
    
    setTimeLongPress(timeLongPress);
    setTypeLongPress(typeLongPress);

    setPtrActionShort(ptrActionShort);
    setPtrActionLong(ptrActionLong);
}

void Button::setTimeLongPress(uint32_t timeLongPress) { this->timeLongPress = timeLongPress; }

uint32_t Button::getTimeLongPress() { return timeLongPress; }

void Button::setTypeLongPress(longPress_t typeLongPress) { this->typeLongPress = typeLongPress; }

longPress_t Button::getTypeLongPress() { return typeLongPress; };

void Button::setPtrActionShort(ptrProcedure ptrActionShort) { this->ptrActionShort = ptrActionShort; }

void Button::setPtrActionLong(ptrProcedure ptrActionLong) { this->ptrActionLong = ptrActionLong; }

int8_t Button::checkPress() {   
    /* Read and save the value for next analysis. */
    uint8_t valueRead = digitalRead(pin);

    /* Cheching if the button is pressed in this moment. */
    if (valueRead == valuePress) {
        /* Checking if is the first press or not. */
        if (!isPressed) {
            //Serial.println("1.1");
            isPressed = true;

            /* Checking if is set the time of long press. If there is a value, will be set the timeout. */
            if (getTimeLongPress() > DEFAULT_LONG_PRESS) {
                timeOut = millis() + getTimeLongPress();
            }

        } else {
            /* Checking if is the long press. */
            if ((millis() >= timeOut) && (getTimeLongPress() > DEFAULT_LONG_PRESS) && !isLongPressed) {
                //Serial.println("2.1");
                actualValue = -1;
                isLongPressed = true;

                /* If there is a pointer to a procedure, will be executed. */
                if (ptrActionLong != NULL) {
                    ptrActionLong();
                    
                    while (valueRead == HIGH); 
                }
            } else if (isLongPressed && (getTypeLongPress() == B_NOTCONTINUOUS)) {
                //Serial.println("2.2");
                actualValue = 0;
            } else {
                //Serial.println("2.3");
                actualValue = 0;
            }
            
        }
    
    /* Else, the button is not pressed in this moment. */
    } else {
        /* Checking if is the short press, verifying if is set the "timeLongPress" or not. */      
        if (isPressed && (((actualValue != -1) && (millis() < timeOut) && !isLongPressed) || (getTimeLongPress() == DEFAULT_LONG_PRESS))) {
            //Serial.println("3.1");
            actualValue = 1;
            isPressed = false;
            isLongPressed = false;

            /* If there is a pointer to a procedure, will be executed. */
            if (ptrActionShort != NULL) {
                ptrActionShort();
            }

        /* Else, is not a press. */
        } else {
            //Serial.println("3.2");
            actualValue = 0;
            isPressed = false;
            isLongPressed = false;
        }
    }

    return actualValue;
}

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
    if (mode == INPUT) {
        valuePress = HIGH;
    } else if (mode == INPUT_PULLUP) {
        valuePress = LOW;
    }
}