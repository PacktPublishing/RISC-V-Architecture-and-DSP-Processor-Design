################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := C:\Users\xrz\Desktop\240117\240117\build-utils\clean

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
all: Example_RV335_EPWM_dma.elf

# Tool invocations
Example_RV335_EPWM_dma.elf: $(OBJECT_FILE)
	@echo 'Building target: $@'
	@echo 'Invoking: RISCV linker'
	"C:\Users\xrz\Desktop\240117\240117\riscv\llvm\bin\clang" $(OBJECT_FILE)  --gcc-toolchain="C:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain" --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -T"C:/Users/xrz/Desktop/240117/240117/workspace/Example_RV335_EPWM_dma/Example_RV335_EPWM_dma/system/link_riscv.ld" -LC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\gcc\riscv32-unknown-elf\lib -o "Example_RV335_EPWM_dma.elf"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ELF_FILE_OUTPUTS)$(OBJECT_FILE) Example_RV335_EPWM_dma.elf
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
