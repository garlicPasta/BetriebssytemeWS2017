mt=arm920t


kernel.bin: kernel.elf
	arm-none-eabi-objcopy -Obinary \
		--set-section-flags \
		.bss=contents,alloc,load,data kernel.elf kernel.bin

vectors.o: vectors.s
	arm-none-eabi-as -mcpu=$(mt) -g vectors.s -o vectors.o

main.o: main.c
	arm-none-eabi-gcc -Wall -Wextra -g -ffreestanding  -mcpu=$(mt) -c main.c -o main.o

my_print.o: my_print.c
	arm-none-eabi-gcc -Wall -Wextra -g -ffreestanding  -mcpu=$(mt) -c my_print.c -o my_print.o

kernel.elf: main.o vectors.o kernel.lds my_print.o
	arm-none-eabi-ld -T kernel.lds main.o my_print.o vectors.o -o kernel.elf

kernel.img: kernel.bin
	mkimage -A arm -T standalone -C none \
		-a 0x20000000 \
		-d kernel.bin kernel.img

clean:
	@rm *.o *.bin *.elf *.img

run: kernel.elf
	qemu-bsprak -kernel kernel.elf

debug: kernel.elf
	qemu-bsprak -s -S -kernel kernel.elf &

