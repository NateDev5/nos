#include "video.h"
#include "../memory/mem.h"
#include "../utils/math.h"

static uint32 currentOffset = 0;

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

void printuint32 (IN uint32 value, IN int32 base) {
    char str[256];
    itoa(value, str, base);
    println(str, BWHITE);
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

void errorScreen (IN const cstr msg) {
    clearScreen();
    print("ERROR: ", BLACK);
    println(msg, BLACK);
    setBackgroundColor(LRED);
}

void clearScreen()
{
    uint8 *videoMem = (uint8 *)BASE_VID_MEM;
    memset((PTRMEM)BASE_VID_MEM, 0x0, SCRN_SIZE * 2);
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