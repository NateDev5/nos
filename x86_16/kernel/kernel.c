void kmain(void)
{
    *(char *)0xb8000 = 'N';
    for(;;){}
    return;
}