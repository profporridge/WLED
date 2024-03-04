#include "TTGO-eink_menu.h"

void setup() {
    setupMenu();
}

void loop() {
    taskManager.runLoop();
}

void CALLBACK_FUNCTION UpdateColor(int id) {
    // TODO - your menu change code
}

void CALLBACK_FUNCTION UpdateAudio(int id) {
    // TODO - your menu change code
}

// see tcMenu list documentation on thecoderscorner.com
int CALLBACK_FUNCTION fnEffectsRtCall(RuntimeMenuItem * item, uint8_t row, RenderFnMode mode, char * buffer, int bufferSize) {
   switch(mode) {
    case RENDERFN_INVOKE:
        // TODO - your code to invoke goes here - row is the index of the item
        return true;
    case RENDERFN_NAME:
        // TODO - each row has it's own name (for list items row==LIST_PARENT_ITEMPOS is back)
        ltoaClrBuff(buffer, row, 3, NOT_PADDED, bufferSize);
        return true;
    case RENDERFN_VALUE:
        // TODO - return a value for the row (for list items row==LIST_PARENT_ITEMPOS is back)
        buffer[0] = 'V'; buffer[1]=0;
        fastltoa(buffer, row, 3, NOT_PADDED, bufferSize);
        return true;
    case RENDERFN_EEPROM_POS: return 0xFFFF; // lists are generally not saved to EEPROM
    default: return false;
    }
}
