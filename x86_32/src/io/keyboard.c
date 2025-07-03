#include "keyboard.h"
#include "../video/video.h"
#include "../utils/math.h"
#include "io.h"

// https://wiki.osdev.org/Serial_Ports#Wires,_Pins,_Connectors_and_the_like

bool initKeyboard () {
    // disable interupts we dont want them
    outb(KEYBOARD_PORT + 1, 0x00);

    // set the most significant bit of the Line Control Register to allow modifications to the divisor
    outb(KEYBOARD_PORT + 3, 0x80); // 0x80 is 1000000 (binary)

    // set divisor
    // divisor is 1 which is a baud rate of 115200 (115200 / 1 = 115200)
    outb(KEYBOARD_PORT, 0x01); // least significant bit
    outb(KEYBOARD_PORT + 1, 0x00); // most significant bit

    // clear the Line Control Register
    outb(KEYBOARD_PORT + 3, 0x00);

    // set the line control register again (00000110: 7 bits per character, 1 stop bit) 
    outb(KEYBOARD_PORT + 3, 0x06); // 8N1 (8 bits, no parity bit, 1 stop bit)

    // set the FIFO 
    // - clear both transmit and receive FIFO, 
    // - enable FIFO 
    // - set the interupt trigger level (how much data much be received to trigger the interupt) to 14 bytes)
    outb(KEYBOARD_PORT + 2, 0xC7); // 11000111

    // modem control register
    // enable IRQ and DTR/RTS
    // this is need for a loopback (testing the port)
    outb(KEYBOARD_PORT + 4, 0x0B);  // 00001011

    // set in loopback mode
    outb(KEYBOARD_PORT + 4, 0x1E);

    // send test byte (will not reply the same if faulty)
    outb(KEYBOARD_PORT, 0xAE);
    if(inb(KEYBOARD_PORT) != 0xAE)
        return false;
    
    // if the port works set it to normal mode
    // no loopback with IRQ enabled as well as OUT 1 and 2 enabled
    outb(KEYBOARD_PORT + 4, 0x0F); // 00001111

    return true;
}