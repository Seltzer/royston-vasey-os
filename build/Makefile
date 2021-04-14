###
# Intended to be run from a Debian system using a GCC cross-compiler
###

# Standard paths
C_OBJDIR = ../out/obj/c

# Filelists for kernel compilation
C_SOURCES = $(wildcard ../kernel/*.c)
C_HEADERS = $(wildcard ../kernel/*.h)
C_OBJ = $(C_SOURCES:%.c=%.o)

# Compiler stuff
CC = "i686-elf-gcc"
C_FLAGS = "-ffreestanding"


# All target
all: os.img

debug: 
	$(info headers $(C_HEADERS))		
	$(info sources $(C_SOURCES))	
	$(info obj $(C_OBJ))

# Run target
run: all
	bochs

# Agglomerated OS image
os.img: boot-loader.bin kernel.bin
	cat $^ > os.img

# Boot loader
boot-loader.bin: ../kernel/boot/boot-loader/boot-loader.asm
	nasm $< -f bin -I ../kernel/boot/boot-loader/ -o $@

# kernel.bin = kernel entry + compiled C kernel object files
kernel.bin: kernel-entry.o ${C_OBJ}		
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

# kernel-entry
kernel-entry.o: ../kernel/boot/kernel-entry.asm
	nasm $< -f elf -o $@

# Generic rule for compiling C files using our cross-compiler
%.o: %.c ${C_HEADERS}
	$(CC) $(C_FLAGS) -c $< -o $@