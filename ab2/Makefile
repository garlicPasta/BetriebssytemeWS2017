CC = arm-none-eabi-gcc
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy

LDSCRIPT = kernel.lds

OBJ = system/main.o system/vectors.o
OBJ += lib/my_print.o

mt=arm920t
CFLAGS = -Wall -Wextra -g -ffreestanding  -mcpu=$(mt)
CPPFLAGS = -Iinclude

DEP=$(OBJ:.o=.d)

-include $(DEP)

kernel.elf: $(OBJ) $(LDSCRIPT)
	$(LD) -T $(LDSCRIPT) $(OBJ) -o kernel.elf

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) -mcpu=$(mt) -g -c $< -o $@

kernel.bin: kernel.elf
	$(OBJCOPY) -Obinary --set-section-flags .bss=contents,alloc,load,data $< $@

.PHONY:clean
clean:
	@rm -f *.bin *.elf
	@rm -f $(OBJ)

.PHONY:run
run: kernel.elf
	qemu-bsprak -kernel kernel.elf

.PHONY:debug
debug: kernel.elf
	qemu-bsprak -s -S -kernel kernel.elf &

