################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := D:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\build-utils\clean

# All of the sources participating in the build are defined here
-include sources.mk
-include system/subdir.mk
-include src/subdir.mk
-include routines/source/subdir.mk
-include subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: watchdog.elf

# Tool invocations
watchdog.elf: $(OBJECT_FILE)
	@echo 'Building target: $@'
	@echo 'Invoking: RISCV linker'
	"D:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\llvm\bin\clang" $(OBJECT_FILE)  --gcc-toolchain="D:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\gnu-toolchain" --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -T"D:/IDE/240223/240223/workspace/watchdog/system/link_riscv.ld" -LD:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\gnu-toolchain\riscv32-unknown-elf\lib -lc -lsemihost -o "watchdog.elf"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ELF_FILE_OUTPUTS)$(OBJECT_FILE) watchdog.elf
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
