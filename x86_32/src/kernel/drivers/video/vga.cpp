#include <kernel/drivers/video/vga.h>
#include <kernel/memory/mem.h>

#include <kernel/library/string.h>
#include <kernel/library/panic.h>
#include <kernel/library/stdarg.h>
#include <kernel/library/arrayutils.h>

#include <kernel/library/log.h>

#include <utils/math.h>
#include <utils/asm.h>

namespace Drivers::VGA {
    static uint16 currentOffset = 0;

    void modifyRegister (IN uint8 reg, IN uint8 data) {
        outb(ADDRESS_REGISTER_PORT, reg);
        outb(DATA_REGISTER_PORT, data);
    }

    void enableCursor (IN CursorStyle cursorStyle) {
        uint8 maxScanLines = inb(MAX_SCAN_LINE_REGISTER);
        maxScanLines &= FIRST_5_BIT_MASK;
        switch (cursorStyle)
        {
        case UNDER: 
            modifyRegister(CURSOR_START_REGISTER, 0xD);
            modifyRegister(CURSOR_END_REGISTER, maxScanLines);
            break;
        case LARGE:
            modifyRegister(CURSOR_START_REGISTER, 0);
            modifyRegister(CURSOR_END_REGISTER, maxScanLines);
        }
    }

    void disableCursor () {
        modifyRegister(CURSOR_START_REGISTER, CURSOR_DISABLED); // 0010 0000
    }

    void setCursorPos (IN uint16 offset) {
        modifyRegister(CURSOR_LOCATION_HIGH_REGISTER, (offset >> 8) & 0xFF);
        modifyRegister(CURSOR_LOCATION_LOW_REGISTER,  offset & 0xFF);
    }

    void fputchar (IN int8 _char, IN uint8 format) {
        if (_char == NULL) Kernel::panic("(putcharf) _char is null");
        
        if (currentOffset >= SCRN_SIZE * 2)
            return;
        
        if (_char == '\n')
        {
            int offsetForNewLine = currentOffset % (SCRN_WIDTH * 2);
            currentOffset += (SCRN_WIDTH * 2) - offsetForNewLine;
            return;
        }
        
        uint8 *videoMem = (uint8 *)BASE_VID_MEM;
        videoMem[currentOffset] = _char;
        videoMem[currentOffset + 1] = format;
        currentOffset += 2;
        
        //setCursorPos(currentOffset / 2);
    }

    void putchar (IN int8 _char) {
        fputchar(_char, BWHITE);
    }

    void fputstr (IN cstr string, IN uint8 format) {
        if (string == NULL) Kernel::panic("(fputstr) string is null");
    
        for (uint32 i = 0; (string[i] != 0) && (i < SCRN_SIZE); i++)
            fputchar(string[i], format);
    }
    void putstr (IN cstr string) {
        fputstr(string, BWHITE);
    }

    void fputcharAt (IN int8 _char, IN uint16 offset) {
    }

    void popchar () {
        uint8 *videoMem = (uint8 *)BASE_VID_MEM;
        videoMem[currentOffset - 1] = 0;
        videoMem[currentOffset - 2] = 0;
        currentOffset -= 2;
    }

    void removecharAt (IN uint16 offset) {
        if(offset > SCRN_SIZE) return;

        uint8 *videoMem = (uint8 *)BASE_VID_MEM;

        for(uint16 i = offset; i < currentOffset; i += 2) {
            videoMem[i] = videoMem[i+2];
            videoMem[i+1] = videoMem[i+3];

            videoMem[i+2] = NULL;
            videoMem[i+3] = BASE_FMT;
        }

        currentOffset -= 2;
    }

    void setBackgroundColor(IN uint8 color)
    {
        uint8 *videoMem = (uint8 *)BASE_VID_MEM;

        for (int32 i = 0; i <= SCRN_SIZE; i++)
        {
            // 0x0F is a mask for 00001111 so applying a logical AND will get only the four bytes
            int32 curForeColor = videoMem[i * 2 + 1] & 0x0F;
            videoMem[i * 2 + 1] = color << 4 | curForeColor;
        }
    }

    void setForegroundColor(IN uint8 color)
    {
        uint8 *videoMem = (uint8 *)BASE_VID_MEM;
        for (int32 i = 0; i <= SCRN_SIZE; i++)
        {
            // 0x0F is a mask for 11110000 so applying a logical AND will get only the four bytes
            int32 curBackColor = videoMem[i * 2 + 1] & 0xF0;
            videoMem[i * 2 + 1] = curBackColor | color;
        }
    }

    void clearScreen()
    {
        currentOffset = 0;

        uint8 *videoMem = (uint8 *)BASE_VID_MEM;

        for(uint32 ui = 0; ui < SCRN_SIZE * 2; ui++) {
            if(ui % 2) videoMem[ui] = BASE_FMT;
            else videoMem[ui] = 0;
        }
    }

    void test()
    {
        uint8 *videoMem = (uint8 *)BASE_VID_MEM;
        for (int32 i = 0; i <= SCRN_SIZE; i++)
        {
            videoMem[i * 2] = 'A' + i;
            videoMem[i * 2 + 1] = i;
        }
    }

    uint16 getCurrentOffset () {
        return currentOffset;
    }
}