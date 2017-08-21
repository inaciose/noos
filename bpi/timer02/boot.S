// To keep this in the first portion of the binary.
.section ".text.boot"
 
// Make _start global.
.global _start 
_start:
    // We enter execution in supervisor mode. For more information on
    // processor modes see ARM Section A2.2 (Processor Modes)
    // Change to supervisor (SVC) mode anyway
    cps #0x13    
    
    // define stack pointer base
    mov r4, #0x80000000
    // set SVC stack at offset 0x2000
    add sp, r4, #0x2400

    // r0, r1, r2 are arguments for main
    bl      main

    // If main does return for some reason, just catch it and stay here.
_inf_loop:
    b       _inf_loop
