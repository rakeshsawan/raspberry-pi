#ARMGNU ?= aarch64-linux-gnu
ARMGNU ?= aarch64-unknown-linux-gnu

COPS = -g -Wall -nostdlib -nostartfiles -ffreestanding -Iinclude -mgeneral-regs-only
ASMOPS = -g -Iinclude 

BUILD_DIR = build
SRC_DIR = .

all : kernel8.img kernel8.list

clean :
	    rm -rf $(BUILD_DIR) *.img kernel.list

$(BUILD_DIR)/%_c.o: $(SRC_DIR)/%.c
	    mkdir -p $(@D)
	        $(ARMGNU)-gcc $(COPS) -MMD -c $< -o $@

$(BUILD_DIR)/%_s.o: $(SRC_DIR)/%.S
	    $(ARMGNU)-gcc $(ASMOPS) -MMD -c $< -o $@

C_FILES = $(wildcard $(SRC_DIR)/*.c)
ASM_FILES = $(wildcard $(SRC_DIR)/*.S)
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
	OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)

DEP_FILES = $(OBJ_FILES:%.o=%.d)
	-include $(DEP_FILES)

kernel8.img: $(SRC_DIR)/linker.ld $(OBJ_FILES)
	    $(ARMGNU)-ld -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/kernel8.elf  $(OBJ_FILES)
	        $(ARMGNU)-objcopy $(BUILD_DIR)/kernel8.elf -O binary kernel8.img
kernel8.list: build/kernel8.elf
	$(ARMGNU)-objdump -D build/kernel8.elf > kernel.list


run:
	qemu-system-aarch64 -M raspi3 -kernel kernel8.img -d in_asm
