mt=arm920t


kernel.bin: kernel.elf
	#arm-none-eabi-objcopy -O binary test.elf kernel.bin
	arm-none-eabi-objcopy -Obinary \
		--set-section-flags \
		.bss=contents,alloc,load,data kernel.elf kernel.bin

startup.o: startup.s
	arm-none-eabi-as -mcpu=$(mt) -g startup.s -o startup.o

main.o: main.c
	arm-none-eabi-gcc -Wall -Wextra -ffreestanding  -mcpu=$(mt) -O2 -c main.c -o main.o

kernel.elf: main.o startup.o kernel.lds
	arm-none-eabi-ld -T kernel.lds main.o startup.o -o kernel.elf

kernel.img: kernel.bin
	mkimage -A arm -T standalone -C none \
		-a 0x20000000 \
		-d kernel.bin kernel.img

clean:
	rm *.o *.bin *.elf *.img

run: kernel.elf
	#qemu-system-arm -M versatilepb -m 128M -nographic -kernel kernel.bin
	qemu-bsprak -kernel kernel.elf
