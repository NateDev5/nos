#include <kernel/drivers/video/vga.h>
#include <kernel/memory/mem.h>

#include <kernel/library/string.h>
#include <kernel/library/panic.h>
#include <kernel/library/stdarg.h>

#include <utils/math.h>
#include <utils/asm.h>

namespace Drivers::VGA {
    static uint32 currentOffset = 0;

    void disableCursor () {
        outb(ADDRESS_REGISTER_PORT, CURSOR_START_REGISTER);
        outb(DATA_REGISTER_PORT, 0x10); // 0001 0000
    }

    void putchar (IN int8 _char) {
        fputchar(_char, BWHITE);
    }

    void fputchar (IN int8 _char, IN uint8 format) {
        if (_char == NULL) Kernel::panic("(putcharf) _char is null");

        if (currentOffset >= SCRN_SIZE)
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
    }

    void putstr (IN cstr string) {
        fputstr(string, BWHITE);
    }

    void fputstr (IN cstr string, IN uint8 format) {
        if (string == NULL) Kernel::panic("(fputstr) string is null");

        for (uint32 i = 0; (string[i] != 0) && (i < SCRN_SIZE); i++)
            fputchar(string[i], format);
    }

    /*
    void printchar(IN int8 _char, IN uint8 format)
    {
        if (_char == NULL)
            return;

        if (currentOffset >= SCRN_SIZE)
            return;

        uint8 *videoMem = (uint8 *)BASE_VID_MEM;
        videoMem[currentOffset] = _char;
        videoMem[currentOffset + 1] = format;
        currentOffset += 2;
    }

    void print(IN const cstr msg, IN uint8 format)
    {
        if (msg == NULL)
            return;

        uint8 *videoMem = (uint8 *)BASE_VID_MEM;
        int32 offset = 0;

        for (int32 i = 0; (msg[i] != 0) && (i < SCRN_SIZE); i++)
        {
            if (msg[i] == '\n')
            {
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

    void println(IN const cstr msg, IN uint8 format)
    {
        print(msg, format);
        print("\n", format);
    }

    void printuint32 (IN uint32 value, IN int32 base, IN uint8 format) {
        char str[256];
        Math::itoa(value, str, base);
        print(str, format);
    }

    void printf(IN cstr msg, IN uint8 format, IN ...) {
        if (msg == NULL)
            return;

        int8 outMsg[STR_MAX_LEN];

        va_list args;
        va_start(args, format);

        str arg = va_arg(args, str);
        printuint32((uint32)&arg[0], 16, BWHITE);
        //Library::strfmt(msg, outMsg, args);

        va_end(args);

        print(outMsg, format);
    }
    */

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

    /*
    void errorScreen (IN const cstr msg) {
        Drivers::VGA::clearScreen();
        print("ERROR: ", BLACK);
        println(msg, BLACK);
        setBackgroundColor(LRED);
    }
    */

    void clearScreen()
    {
        Memory::memset((PTRMEM)BASE_VID_MEM, 0x0, SCRN_SIZE * 2);
        currentOffset = 0;
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
}