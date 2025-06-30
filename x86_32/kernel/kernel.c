void test();
void print(const char* msg, int format);
void println(const char* msg, int format);
void setBackgroundColor (int color);
void setForegroundColor (int color);
void clearScreen ();

#define BASE_VID_MEM   0xB8000
#define SCRN_WIDTH     80
#define SCRN_HEIGHT    25
#define SCRN_SIZE      SCRN_HEIGHT * SCRN_WIDTH

#define BG_BLACK       0x0
#define BG_BLUE        0x1
#define BG_GREEN       0x2
#define BG_CYAN        0x3
#define BG_RED         0x4
#define BG_MAGENTA     0x5
#define BG_BROWN       0x6
#define BG_WHITE       0x7
#define BG_GRAY        0x8
#define BG_LBLUE       0x9
#define BG_LGREEN      0xA
#define BG_LCYAN       0xB
#define BG_LRED        0xC
#define BG_LMAGENTA    0xD
#define BG_YELLOW      0xE
#define BG_BWHITE      0xF

#define FG_BLACK       0x00
#define FG_BLUE        0x01
#define FG_GREEN       0x02
#define FG_CYAN        0x03
#define FG_RED         0x04
#define FG_MAGENTA     0x05
#define FG_BROWN       0x06
#define FG_WHITE       0x07
#define FG_GRAY        0x08
#define FG_LBLUE       0x09
#define FG_LGREEN      0x0A
#define FG_LCYAN       0x0B
#define FG_LRED        0x0C
#define FG_LMAGENTA    0x0D
#define FG_YELLOW      0x0E
#define FG_BWHITE      0x0F

// shift 4 bits to the left so the first 4 bits are the first color and then mask the last 4 bits to the second color
#define BACK_FORE(back,fore) (back) | (fore)

int currentOffset = 0;

void kmain()
{
    //test();
    //print("Hello", BWHITE);
    print("Hello world from NOS!\nLets goo!!!\n", BG_CYAN | FG_MAGENTA);
    //clearScreen();
    //println("This is a working println!", FG_BWHITE);
    //println("Hell yeah", FG_BWHITE);
    //setBackgroundColor(LBLUE);
    //setForegroundColor(LMAGENTA);

    //clearScreen();
}

void test () {
    unsigned char *videoMem = (unsigned char *)BASE_VID_MEM;
    for(int i = 0; i <= SCRN_SIZE; i++) {
        videoMem[i * 2] = 'A' + i;
        videoMem[i * 2 + 1] = i;
    }
}

void print(const char* msg, int format) {
    unsigned char *videoMem = (unsigned char *)BASE_VID_MEM;
    int offset = 0;
    for (int i = 0; msg[i] != 0; i++)
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

void println(const char* msg, int format) {
    print(msg, format);
    print("\n", format);
}

void setBackgroundColor (int color) {
    unsigned char *videoMem = (unsigned char *)BASE_VID_MEM;
    for(int i = 0; i <= SCRN_SIZE; i++) {
        videoMem[i * 2 + 1] = color << 4 | videoMem[i * 2 + 1];
    }
}

void setForegroundColor (int color) {
    unsigned char *videoMem = (unsigned char *)BASE_VID_MEM;
    for(int i = 0; i <= SCRN_SIZE; i++) {
        videoMem[i * 2 + 1] = videoMem[i * 2 + 1] << 4 | color;
    }
}

void clearScreen () {
    unsigned char *videoMem = (unsigned char *)BASE_VID_MEM;
    for(int i = 0; i <= SCRN_SIZE; i++) {
        videoMem[i * 2] = 0x00;
        videoMem[i * 2 + 1] = 0x00;
    }
    currentOffset = 0;
} 