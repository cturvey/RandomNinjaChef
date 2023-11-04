/*
startup.s code
;*****************************************************************************

HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler          [WEAK]

                TST     lr, #4          ; Determine correct stack
                ITE     EQ
                MRSEQ   R0, MSP         ; Read MSP (Main)
                MRSNE   R0, PSP         ; Read PSP (Process)

                MOV     R1, R4          ; Registers R4-R6, as parameters 2-4 of the function called
                MOV     R2, R5
                MOV     R3, R6          ;  sourcer32@gmail.com

                EXTERN  hard_fault_handler_c
                B       hard_fault_handler_c
                ENDP

;*****************************************************************************

; Equivalent for GNU/GAS used with STM32CubeIDE etc

    .section  .text.HardFault_Handler,"ax",%progbits
HardFault_Handler:

                /* Determine correct stack */

                TST     lr, #4
                ITE     EQ
                MRSEQ   R0, MSP         /* Read MSP (Main) */
                MRSNE   R0, PSP         /* Read PSP (Process) */

                MOV     R1, R4
                MOV     R2, R5
                MOV     R3, R6

                B       hard_fault_handler_c

  .size  HardFault_Handler, .-HardFault_Handler

;*****************************************************************************

;Note to self, Cortex-M0(+) startup.s code for Keil

HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler
                EXTERN  hard_fault_handler_c
 
                MOV     R1, LR
                LDR     R0, =hard_fault_handler_c
                MOV     LR, R0
                MOVS    R0, #4          ; Determine correct stack
                TST     R0, R1
                MRS     R0, MSP         ; Read MSP (Main)
                BEQ     .+6             ; BEQ 2, MRS R0,PSP 4
                MRS     R0, PSP         ; Read PSP (Process)
 
                MOV     R1, R4          ; Registers R4-R6, as parameters 2-4 of the function called
                MOV     R2, R5
                MOV     R3, R6          ;  sourcer32@gmail.com
 
                BX      LR
                ENDP

;*****************************************************************************
*/

//****************************************************************************

void hard_fault_handler_c(unsigned int * hardfault_args, unsigned int r4, unsigned int r5, unsigned int r6)
{
  printf("\n[Hard Fault]\n"); // After Joseph Yiu

  printf("r0 = %08X, r1 = %08X, r2 = %08X, r3 = %08X\n",
    hardfault_args[0], hardfault_args[1], hardfault_args[2], hardfault_args[3]);
  printf("r4 = %08X, r5 = %08X, r6 = %08X, sp = %08X\n",
    r4, r5, r6, (unsigned int)&hardfault_args[8]);
  printf("r12= %08X, lr = %08X, pc = %08X, psr= %08X\n",
    hardfault_args[4], hardfault_args[5], hardfault_args[6], hardfault_args[7]);

  if (__CORTEX_M >= 3)
    printf("bfar=%08X, cfsr=%08X, hfsr=%08X, dfsr=%08X, afsr=%08X\n",
      *((volatile unsigned int *)(0xE000ED38)),
      *((volatile unsigned int *)(0xE000ED28)),
      *((volatile unsigned int *)(0xE000ED2C)),
      *((volatile unsigned int *)(0xE000ED30)),
      *((volatile unsigned int *)(0xE000ED3C)) );

  extern void *__initial_sp;

  { // Stack Dump
    int i = 0;
    uint32_t *p = (uint32_t *)&hardfault_args[8];
    uint32_t *q = (uint32_t *)&__initial_sp;
    while((p < q) && (i < 32))
    {
      if ((i++ & 7) == 0) putchar('\n');
      printf(" %08X", *p++);
    }
    putchar('\n');
  }

  { // Instruction Dump
    int i = 8;
    uint16_t *p = (uint16_t *)(hardfault_args[6] - (i*2)); // PC, 8 opcodes earlier
    putchar('\n');
    while(i--)
      printf(" %04X", *p++);
    printf("(%04X)", *p++);
    i = 5;
    while(i--)
      printf("%04X ", *p++);
    putchar('\n');
  }

  while(1)
  {
    //USART1_IRQHandler(); // Pump buffering
  }
}

//****************************************************************************
