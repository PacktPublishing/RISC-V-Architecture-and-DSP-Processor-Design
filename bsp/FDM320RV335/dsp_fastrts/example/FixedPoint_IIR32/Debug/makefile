################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := D:\CodeCanvas_2406041022\build-utils\clean

# All of the sources participating in the build are defined here
-include sources.mk
-include src/source/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: FixedPoint_IIR32.elf

# Tool invocations
FixedPoint_IIR32.elf: $(OBJECT_FILE)
	@echo 'Building target: $@'
	@echo 'Invoking: RISCV linker'
	"D:\CodeCanvas_2406041022\riscv\llvm\bin\clang" $(OBJECT_FILE)  --gcc-toolchain="D:\CodeCanvas_2406041022\riscv\gnu-toolchain" --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -T"D:/FDMTEK_RV335/dsp_fastrts/example/FixedPoint_IIR32/system/link_riscv.ld" -LD:\CodeCanvas_2406041022\riscv\gnu-toolchain\riscv32-unknown-elf\lib -L"D:\FDMTEK_RV335\dsp_fastrts\example\FixedPoint_IIR32\lib" -lm -lrv335dspmath -o "FixedPoint_IIR32.elf"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ELF_FILE_OUTPUTS)$(OBJECT_FILE) FixedPoint_IIR32.elf
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
