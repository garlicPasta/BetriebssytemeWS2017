CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

LDSCRIPT = kernel.lds

OBJ =  system/main.o system/vectors.o system/cpu.o system/cpu_asm.o
OBJ += system/interrupts.o system/scheduler.o
OBJ += lib/my_print.o lib/math.o lib/threading.o lib/kernel.o
OBJ += driver/debug_unit.o driver/aic.o driver/system_timer.o

mt=arm920t
CFLAGS = -Wall -Wextra -g -ffreestanding -mcpu=$(mt) -O2
CPPFLAGS = -Iinclude

DEP=$(OBJ:.o=.d)

-include $(DEP)

kernel.elf: $(OBJ) $(LDSCRIPT) 
	$(LD) -T $(LDSCRIPT) $(OBJ) -o kernel.elf

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CPPFLAGS) $(CFLAGS) -g -c $< -o $@

kernel.bin: kernel.elf
	$(OBJCOPY) -Obinary --set-section-flags .bss=contents,alloc,load,data $< $@

.PHONY:clean
clean:
	@rm -f *.bin
	@rm -f $(OBJ)

.PHONY:run
run: kernel.elf
	qemu-bsprak -kernel kernel.elf

.PHONY:debug
debug: kernel.elf
	qemu-bsprak -s -S -kernel kernel.elf & arm-none-eabi-gdb -ex 'target remote localhost:1234' -ex 'symbol-file $<'
