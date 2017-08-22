void greenled_config( void ) {
  // Configure port PH24 (green led) for output
  asm ("\n"
    "mov r0,  #0x00000001 \n"
    "ldr r3, =0x01C20908 \n"
    "str r0, [r3] \n");
}

void greenled_on( void ) {
  
  // Turn led on PH24
  asm (" \n"
    "mov r0,  #0x01000000 \n"
    "ldr r3, =0x01C2090C \n"
    "str r0, [r3]");
}

void greenled_off( void ) {
  // Turn led off PH24
  asm (" \n"
    "mov r0,  #0x00000000 \n"
    "ldr r3, =0x01C2090C \n"
    "str r0, [r3]");
}

