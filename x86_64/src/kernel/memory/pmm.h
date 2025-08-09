//https://github.com/dreamportdev/Osdev-Notes/blob/master/04_Memory_Management/02_Physical_Memory.md
//https://blog.pepsipu.com/posts/pmm-for-oses
//http://www.brokenthorn.com/Resources/OSDev17.html

/*
* The physical memory manager is responsible for keeping track of used/free physical memory maps
*/

#pragma once

#define PAGE_SIZE   4096 // 4kb
