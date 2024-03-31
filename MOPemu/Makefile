CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
MACH=cortex-m3
CFLAGS= -c -mcpu=$(MACH) -mthumb -std=gnu11 -ffreestanding -Wall -g -O0 -Wa,-adhlns="$@.lst"
LDFLAGS= -T stm32_ls.ld -specs=nosys.specs -nostdlib -lgcc -lc -Wl,-Map=blinky.map


CFLAGS += -Wno-shift-count-overflow -Wno-shift-count-negative

TARGET=blinky

all: $(TARGET).bin

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $^ $@
	arm-none-eabi-size $^

$(TARGET).elf: $(TARGET).o delay.o led.o printf.o stm32_startup.o
	$(CC) $(LDFLAGS) -o $@ $^

.PHONY: clean
clean:
	rm -f *.o *.elf *.bin *.map *.lst

.PHONY: flash
flash: all
	@st-flash write $(TARGET).bin 0x08000000 2>&1 | grep "jolly"

.PHONY: reset
reset:
	@st-flash reset > /dev/null 2>&1

.PHONY: debughost
debughost:
	st-util

.PHONY: gdb
gdb: $(TARGET).elf
	arm-none-eabi-gdb $(TARGET).elf


TARGET = blinky

# # Default target chip.
# MCU ?= STM32F103C8

# # Define target chip information.
# 	# MCU_FILES = STM32F103C8
# 	MCU_FILES = stm32_ls
# 	ST_MCU_DEF = STM32F103xB
# 	MCU_CLASS = F1

# #
# # Generic STM32 makefile:
# #
# MCU_SPEC = cortex-m3

# # Toolchain definitions (ARM bare metal defaults)
# #TOOLCHAIN = /usr/bin/
# TOOCHAIN =
# CC = $(TOOLCHAIN)arm-none-eabi-gcc
# AS = $(TOOLCHAIN)arm-none-eabi-as
# LD = $(TOOLCHAIN)arm-none-eabi-ld
# OC = $(TOOLCHAIN)arm-none-eabi-objcopy
# OD = $(TOOLCHAIN)arm-none-eabi-objdump
# OS = $(TOOLCHAIN)arm-none-eabi-size

# # Assembly directives.
# ASFLAGS += -c
# ASFLAGS += -O0
# ASFLAGS += -mcpu=$(MCU_SPEC)
# ASFLAGS += -mthumb
# ASFLAGS += -Wall
# # (Set error messages to appear on a single line.)
# ASFLAGS += -fmessage-length=0
# ASFLAGS += -DVVC_$(MCU_CLASS)

# # C compilation directives
# CFLAGS += -mcpu=$(MCU_SPEC)
# CFLAGS += -mthumb
# CFLAGS += -msoft-float
# CFLAGS += -mfloat-abi=soft
# CFLAGS += -Wall
# CFLAGS += -g
# CFLAGS += -fmessage-length=0
# CFLAGS += -ffunction-sections
# CFLAGS += -fdata-sections
# CFLAGS += --specs=nosys.specs
# CFLAGS += -D$(ST_MCU_DEF)
# CFLAGS += -D$(MCU_FILES)
# CFLAGS += -DVVC_$(MCU_CLASS)

# # Linker directives.
# #LSCRIPT = ./ld/$(MCU_FILES).ld
# LSCRIPT = $(MCU_FILES).ld
# LFLAGS += -mcpu=$(MCU_SPEC)
# LFLAGS += -mthumb
# LFLAGS += -mhard-float
# LFLAGS += -mfloat-abi=hard
# LFLAGS += -mfpu=fpv4-sp-d16
# LFLAGS += -Wall
# LFLAGS += --specs=nosys.specs
# LFLAGS += -lgcc
# LFLAGS += -Wl,--gc-sections
# LFLAGS += -Wl,-L./ld
# LFLAGS += -T$(LSCRIPT)

# # AS_SRC   =  ./$(ST_MCU_DEF)_vt.S
# C_SRC    =  ./blinky.c delay.c led.c stm32_startup.c

# INCLUDE  =  -I./
# # INCLUDE  += -I./device_headers

# OBJS  = $(AS_SRC:.S=.o)
# OBJS += $(C_SRC:.c=.o)

# .PHONY: all
# all: $(TARGET).bin

# # ./$(ST_MCU_DEF)_vt.S:
# # 	python generate_vt.py $(ST_MCU_DEF) $(MCU_SPEC)

# %.o: %.S
# 	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

# %.o: %.c
# 	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

# $(TARGET).elf: $(OBJS)
# 	$(CC) $^ $(LFLAGS) -o $@

# $(TARGET).bin: $(TARGET).elf
# 	$(OC) -S -O binary $< $@
# 	$(OS) $<

# .PHONY: clean
# clean:
# 	rm -f $(OBJS)
# 	rm -f $(ST_MCU_DEF)_vt.S
# 	rm -f $(TARGET).elf
# 	rm -f $(TARGET).bin



