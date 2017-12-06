ENTRY(vectors_start)
SECTIONS
{
 . = 0x20000000;
 .text : {
 system/vectors.o
 *(.text .rodata)
 }
 .data : { *(.data) }
 .bss : { *(.bss) }
}
