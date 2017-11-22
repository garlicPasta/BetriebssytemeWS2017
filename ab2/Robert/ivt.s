.section .init
.global _ivt
_ivt:
    	b 0x2
	b _und
	b _swi
	b _pre
	b _dat
	b _irq

	b _fiq
	

	
_und:
	bl c_entry
_swi:
	b 0x0

_pre:

_dat:

_irq:

_fiq:
















