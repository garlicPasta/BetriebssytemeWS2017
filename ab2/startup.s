.section .init
.global _start
_start:
    B Reset_Handler
    B . /* Undefined */
    B . /* SWI */
    B . /* Prefetch Abort */
    B . /* Data Abort */
    B . /* reserved */
    ldr pc,[pc,#-0xF20]
    B .

Reset_Handler:
  BL c_entry
  B .
