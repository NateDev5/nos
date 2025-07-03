#include "video.h"
#include "../memory/mem.h"

uint32 currentOffset = 0;

void print(const cstr msg, uint8 format) {
    uint8* videoMem = (uint8*)BASE_VID_MEM;
    int32 offset = 0;
    for (int32 i = 0; msg[i] != 0; i++)
    {
        if(msg[i] == '\n') {
            int offsetForNewLine = currentOffset % (SCRN_WIDTH * 2);
            currentOffset += (SCRN_WIDTH * 2) - offsetForNewLine;
            offset = 0;
            continue;
        }

        videoMem[offset + currentOffset] = msg[i];
        videoMem[offset + 1 + currentOffset] = format;
        offset += 2;
    }
    currentOffset += offset;
}

void println(const cstr msg, uint8 format) {
    print(msg, format);
    print("\n", format);
}

void setBackgroundColor (uint8 color) {
    uint8* videoMem = (uint8*)BASE_VID_MEM;

    for(int32 i = 0; i <= SCRN_SIZE; i++) {
        // 0x0F is a mask for 00001111 so applying a logical AND will get only the four bytes
        int32 curForeColor = videoMem[i * 2 + 1] & 0x0F;
        videoMem[i * 2 + 1] = color << 4 | curForeColor;
    }
}

void setForegroundColor (uint8 color) {
    uint8* videoMem = (uint8*)BASE_VID_MEM;
    for(int32 i = 0; i <= SCRN_SIZE; i++) {
        // 0x0F is a mask for 11110000 so applying a logical AND will get only the four bytes
        int32 curBackColor = videoMem[i * 2 + 1] & 0xF0;
        videoMem[i * 2 + 1] = curBackColor | color;
    }
}

void clearScreen () {
    uint8* videoMem = (uint8*)BASE_VID_MEM;
    /*for(int32 i = 0; i <= SCRN_SIZE; i++) {
        videoMem[i * 2] = 0x0;
        videoMem[i * 2 + 1] = 0x0;
    }*/
    memset((uint32*)BASE_VID_MEM, 0x0, SCRN_SIZE);
    currentOffset = 0;
} 

void test () {
    uint8* videoMem = (uint8*)BASE_VID_MEM;
    for(int32 i = 0; i <= SCRN_SIZE; i++) {
        videoMem[i * 2] = 'A' + i;
        videoMem[i * 2 + 1] = i;
    }
}