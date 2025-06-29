void print(const char* msg, int format);

void kmain()
{
    print("Hello world from NOS! hell yeah", 0x07);
}

void print(const char* msg, int format) {
    unsigned char *video_mem = (unsigned char *)0xB8000;
    for (int i = 0; msg[i] != 0; i++)
    {
        video_mem[i * 2] = msg[i];
        video_mem[i * 2 + 1] = 0x07;
    }
}