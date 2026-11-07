# PIC IRQs blocked by masked LAPIC LVT0

**Date fixed:** 2026-11-07
**Time spent:** >8h (not consecutive)

### Summary
Because of the stricter machine state in Limine base revision 5, interrupts were not being delivered to the CPU.

### Symptoms
- The IDT was being loaded correctly
- Interrupts were being raised but never handled
- The PIC was remapped correctly
- No timer interrupts appeared when checking QEMU with `-d int`

### Evidence
> `info pic` showed:
> `irr=03 imr=fc isr=00 irq_base=20`

> `info lapic` showed:
> `LVT0 0x00018700 masked ExtINT`

### Cause
This bug occurred when upgrading from base revision 4 to 5.

### Description
Limine base revision 5 masks LVT entries based on their delivery mode. Since LVT0 uses the ExtINT (External Interrupt) delivery mode, it was masked. This prevented interrupts from the 8259 PIC from reaching the CPU.

Since I was not aware of the LAPIC before this bug occurred, I did not know that the PIC was wired through it. I knew it was a revision problem, but I could not pinpoint it.

### Notes
In the future, when upgrading revisions, compare the specs to make sure the new machine state does not enforce something my code does not take into account.

I used `info pic` and `info lapic` in QEMU to find this bug, with the help of Claude, because I was unable to find it on my own.
