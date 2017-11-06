test.bin: test.elf
	arm-none-eabi-objcopy -O binary test.elf test.bin
startup.o: startup.s
	arm-none-eabi-as -mcpu=arm926ej-s -g startup.s -o startup.o

main.o: main.c
	arm-none-eabi-gcc -c -mcpu=arm926ej-s -g main.c -o main.o

test.elf: main.o startup.o
	arm-none-eabi-ld -T test.ld main.o startup.o -o test.elf

clean:
	rm *.o
	rm *.bin
	rm *.elf

run: test.bin
	qemu-system-arm -M versatilepb -m 128M -nographic -kernel test.bin
