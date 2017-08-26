@;
@; This turns on the ACT LED for the Raspberry Pi 3 Model B v1.2
@;
@; Using GPU's mailbox interface to send a message.

@; Sets the state of the ACT LED
@; state: 1 = on, 0 = off
#include "memlayout.h"

.section .text
.global SetActLEDState
SetActLEDState:
  push {lr} @; Save the point the function should return to
  
  state .req r2
  mov state, r0 @; Move the state into temporary register
  message .req r0
  ldr message, =PropertyInfo @; Load r0 with address of our message buffer
  mov r3, #0
  str r3, [message, #0x4] @; Reset request code
  str r3, [message, #0x10] @; Reset request/response size
  mov r3, #130
  str r3, [message, #0x14] @; Reset pin number
  str state, [message, #0x18] @; Put the requested state in the tag value buffer
  mov r1, #8 @; Set the channel for the following function call
  .unreq message
  .unreq state
  bl MailboxWrite @; Write the message to the mailbox

  mov r0, #8
  bl MailboxRead @; Read from the response from the mailbox
  
  pop {pc} @; Pop the saved LR (return address) into the program counter to return

;@-------------------------------------------------------------------------

.global MailboxWrite
MailboxWrite:
  message .req r0
  add message, r1 @; Add the channel to the message
  mailbox .req r2
  ldr mailbox, =0x3f00b880+KERNBASE @; Load the mailbox's base address into r2

  wait_write$:
    status .req r3
    ldr status, [mailbox, #0x38] @; Load the status of the mailbox (0)
    tst status, #0x80000000 @; Check the status against the FULL bit
    .unreq status
    bne wait_write$ @; Keep checking the mailbox until it isn't full

  str message, [mailbox, #0x20] @; Put the message in the mailbox (1)
  .unreq mailbox
  .unreq message
  mov pc, lr @; Return from the function

@; Read a message from the GPU's mailbox (0)
@;
@; Rust Signature: fn MailboxRead(channel: u32) -> u32
.global MailboxRead
MailboxRead:
  channel .req r0
  mailbox .req r1
  ldr mailbox, =0x3f00b880+KERNBASE @; Load the mailbox's base address into r1

  wait_read$:
    status .req r2;
    ldr status, [mailbox, #0x18] @; Load the mailbox (0) status address
    tst status, #0x40000000 @; Check the status against the EMPTY bit
    .unreq status
    bne wait_read$ @; Keep checking the mailbox until it isn't empty

    mail .req r2
    ldr mail, [mailbox] @; Load the address of the response data
    read_chan .req r3
    and read_chan, mail, #0b1111 @; Extract the channel (the lowest 4 bits)
    teq read_chan, channel @; Test if the channel we extracted is the same
                           @; as the channel we are watching
    .unreq read_chan
    bne wait_read$ @; Keep checking until its a message we are interested in

  mov r0, mail @; Move the mail's address to the function return value
  .unreq channel
  .unreq mail
  .unreq mailbox
  mov pc, lr @; Return from the function

;@-------------------------------------------------------------------------
@; Property tag channel: 8
@; Property tag ID: 0x00038041 (SET_GPIO_STATE)
@; Property tag message: 130 1 (ACT_LED pin number followed by state)

.section .data
.align 4 @; This ensures lowest 4 bits are 0 for the following label
PropertyInfo:
  @; = Message Header =
  .int PropertyInfoEnd - PropertyInfo @; Calculate buffer size
  .int 0 @; Request code: Process Request
  @; = Tag Header =
  .int 0x00038041 @; Tag ID (SET_GPIO_STATE)
  .int 8 @; Value buffer size
  .int 8 @; Request/response size
  @; = Tag Value Buffer =
  .int 130 @; ACT_LED pin number
  .int 1 @; Turn it on
  .int 0 @; End tag
PropertyInfoEnd:
