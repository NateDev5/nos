#include "ps2.h"

#include "../utils/asm.h"
#include "../utils/assert.h"

static uint8 currentFlags = 0;

void initPS2 () {
    // perform self test
    outb(PS2_COMMAND_REGISTER, PS2_SELFTEST_SEND);
    ioWait();
    uint8 selfTestResult = inb(PS2_DATA_PORT);
    assert(selfTestResult == PS2_SELFTEST_RECEIVE, "PS2 Controller self test failed");

    setScanCodeSetPS2();
    enableScanningPS2();
}

void setScanCodeSetPS2 () {
    outb(PS2_COMMAND_REGISTER, 0xD4);
    ioWait();
    outb(PS2_DATA_PORT, PS2_GET_SET_SCAN_CODE_SET);
    uint8 response = inb(PS2_DATA_PORT);
    if(response == PS2_RESEND) {
        setScanCodeSetPS2();
        return;
    }

    outb(PS2_DATA_PORT, PS2_SET_SCAN_CODE_SET_2);
}

void enableScanningPS2 () {
    outb(PS2_DATA_PORT, PS2_ENABLE_SCANNING);
}

uint8 poll () {
    uint8 data = inb(PS2_DATA_PORT);

    return data;
}