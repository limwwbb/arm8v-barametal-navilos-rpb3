ARCH = armv8-a

TARGET = rpb3
ARM_ARCH = rpb3

CC = /opt/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf-gcc 
AS = /opt/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf-as
LD = /opt/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf-ld
OC = /opt/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf-objcopy
OD = /opt/arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-elf/bin/aarch64-none-elf-objdump

OUTDIR=./out

LINKER_SCRIPT = ./build/$(TARGET)/navilos.ld
MAP_FILE = $(OUTDIR)/navilos.map
SYM_FILE = $(OUTDIR)/navilos.sym

ASM_SRCS = $(wildcard ./boot/$(TARGET)/*.S)
ASM_OBJS = $(patsubst ./boot/$(TARGET)/%.S, $(OUTDIR)/%.os, $(ASM_SRCS))

VPATH = boot/$(TARGET) 			\
        hal/$(TARGET)
#        lib				\
	lib/$(ARM_ARCH)		\
        kernel

C_SRCS  = $(notdir $(wildcard ./boot/$(TARGET)/*.c))
C_SRCS += $(notdir $(wildcard ./hal/$(TARGET)/*.c))
#C_SRCS += $(notdir $(wildcard ./lib/*.c))
#C_SRCS += $(notdir $(wildcard ./lib/$(ARM_ARCH)/*.c))
#C_SRCS += $(notdir $(wildcard ./kernel/*.c))
C_OBJS = $(patsubst %.c, $(OUTDIR)/%.o, $(C_SRCS))

INC_DIRS  = -I boot/$(TARGET)	\
            -I include 			\
            -I hal	   			\
            -I hal/$(TARGET)

CFLAGS =  -c -g 

LDFLAGS =  -nostartfiles -nostdlib -nodefaultlibs -static -lgcc 

navilos = $(OUTDIR)/navilos.elf
navilos_bin = $(OUTDIR)/navilos.bin

.PHONY: all clean run debug gdb

all: $(navilos)

clean:
	@rm -fr $(OUTDIR)
	
run: $(navilos)
	qemu-system-aarch64 -M raspi3b -kernel $(navilos) -nographic
	
debug: $(navilos)
	qemu-system-aarch64 -M raspi3b -kernel $(navilos) -nographic -S -gdb tcp::1234,ipv4
	
gdb:
	gdb-multiarch $(navilos)

kill:
	kill -9 `ps aux | grep 'qemu' | awk 'NR==1{print $$2}'`
	
$(navilos): $(C_OBJS) $(ASM_OBJS) $(LINKER_SCRIPT)
	$(LD) -T $(LINKER_SCRIPT) -o $(navilos) $(ASM_OBJS) $(C_OBJS) 
	$(OC) -O binary $(navilos) $(navilos_bin)
	
$(OUTDIR)/%.os: %.S
	mkdir -p $(shell dirname $@)
	$(CC) $(INC_DIRS) $(CFLAGS) -o $@ $<

$(OUTDIR)/%.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) $(INC_DIRS) $(CFLAGS) -o $@ $<
