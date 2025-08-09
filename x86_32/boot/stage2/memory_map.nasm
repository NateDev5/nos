; http://www.uruk.org/orig-grub/mem64mb.html

section .data
    mmap_buffer: resb 2000 ; 100 entries of 20 bytes

section .text
    get_memory_map:
        mov ax, 0
        mov es, ax ; set extra segment to 0
        mov di, mmap_buffer ; set the data point to the start of mmap_buffer
        
        mov ebx, 0 ; set ebx to 0 aka the continuation value (represent the continuation value for the next run of physical mem)

    get_entry:
        mov eax, 0xE820 ; function 
        mov ecx, 20 ; size of an entry
        mov edx, 0x534D4150 ; magic number ('SMAP')

        int 0x15 ; call interrupt

        jc memory_map_error ; if carry flag then there is an error

        cmp eax, 0x534D4150 ; if magic value is different then there is an error
        jne memory_map_error

        add di, 20 ; add 20 to get the start of the next index

        ; if ebx is not 0 (continuation value) then get the next entry
        cmp ebx, 0
        jne get_entry
