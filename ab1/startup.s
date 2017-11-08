.section .init
.global _start
_start:
    bl c_entry
.Lend:
    b .Lend
