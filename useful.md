# https://wiki.osdev.org/GCC_Cross-Compiler

```
unsigned char *videoMem = (unsigned char _)BASE_VID_MEM;
for(int i = 0; i < SCRN_SIZE; i++) {
videoMem[i _ 2] = 'N';
videoMem[i * 2 + 1] = 0x07;
}
```

# https://en.wikipedia.org/wiki/X86_calling_conventions
# https://stanislavs.org/helppc/int_table.html

# https://wiki.osdev.org/Stack_Smashing_Protector
