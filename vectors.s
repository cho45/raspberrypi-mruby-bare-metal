
;@ ------------------------------------------------------------------
;@ ------------------------------------------------------------------

.globl _start
_start:
    ldr pc,reset_handler
    ldr pc,undefined_handler
    ldr pc,swi_handler
    ldr pc,prefetch_handler
    ldr pc,data_handler
    ldr pc,unused_handler
    ldr pc,irq_handler
    ldr pc,fiq_handler

reset_handler:      .word reset
undefined_handler:  .word hang
swi_handler:        .word hang
prefetch_handler:   .word hang
data_handler:       .word hang
unused_handler:     .word hang
irq_handler:        .word irq
fiq_handler:        .word fiq

reset:
    /* Set interrupt handler to radical address from 0x8000
     */
    mov r0,#0x8000
    mov r1,#0x0000
    /* copy machine code 8 bytes at once */
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9} /* load machine code from 0x8000 */
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9} /* store code to 0x0000 */
    /* more 16 bytes */
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}

    /* Initialize each mode stack pointer (sp == r13)
     * (each exception modes have difference sp)
     *  CPSR register lowest 8bit: (page A2-11)
     *      I    [7]   -> IRQ disabled (set 1 to disable)
     *      F    [6]   -> FIQ disabled (set 1 to disable)
     *      T    [5]   -> Always set 0 in ARM state
     *      MODE [4:0] -> Mode bit
     */

    /* IRQ Mode (0b11010010),   */
    mov r0,#0xD2
    msr CPSR_c,r0
    mov sp,#0x8000

    /* FIQ Mode (0b11010001) */
    mov r0,#0xD1
    msr CPSR_c,r0
    mov sp,#0x4000

    /* Supervisor Mode (0b11010011) */
    mov r0,#0xD3
    msr CPSR_c,r0
    mov sp,#0xF000000

    /* Copy initialized variables .data section  (Copy from ROM to RAM) */
    ldr     r1, =__text_end__
    ldr     r2, =__data_start__
    ldr     r3, =__data_end__
1:  cmp     r2, r3
    ldrlo   r0, [r1], #4
    strlo   r0, [r2], #4
    blo     1b

    /* Clear uninitialized variables .bss section  */
    mov     r0, #0
    ldr     r1, =__bss_start__
    ldr     r2, =__bss_end__
2:  cmp     r1, r2
    strlo   r0, [r1], #4
    blo     2b

    /* call C main function */
    bl main


hang: b hang

.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl dummy
dummy:
    bx lr

.globl bss_start
bss_start: .word __bss_start__
.globl bss_end
bss_end: .word __bss_end__
.word __bss_size__
.globl __data_rom_start__
data_rom_start:
.word __data_rom_start__
.globl __data_start__
data_start:
.word __data_start__
.globl __data_end__
data_end:
.word __data_end__
.globl datasize
data_size:
.word __data_size__
.globl text_end
text_end:
.word __text_end__
.globl mem_end
mem_end:
.word __mem_end__

.globl    heap_start
heap_start:
    .word __heap_start__

.globl    heap_limit
heap_limit:
    .word __heap_limit__


.globl enable_irq
enable_irq:
    /* read/modify/write sequence */
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr_c,r0
    bx lr

.globl enable_fiq
enable_fiq:
    /* read/modify/write sequence */
    mrs r0,cpsr
    bic r0,r0,#0x40
    msr cpsr_c,r0
    bx lr

irq:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl timer_irq_handler
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4

fiq:
    nop
    subs pc,lr,#4


;@-------------------------------------------------------------------------
;@-------------------------------------------------------------------------


;@-------------------------------------------------------------------------
;@
;@ Copyright (c) 2012 David Welch dwelch@dwelch.com
;@
;@ Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
;@
;@ The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
;@
;@ THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
;@
;@-------------------------------------------------------------------------
