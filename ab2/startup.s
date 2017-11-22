.section .init
.global _start
_start:
    B Reset_Handler

Reset_Handler:
  BL c_entry
  B .
