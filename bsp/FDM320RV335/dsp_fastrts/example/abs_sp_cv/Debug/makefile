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
all: abs_sp_cv.elf

# Tool invocations
abs_sp_cv.elf: $(OBJECT_FILE)
	@echo 'Building target: $@'
	@echo 'Invoking: RISCV linker'
	"D:\CodeCanvas_2406041022\riscv\llvm\bin\clang" $(OBJECT_FILE)  --gcc-toolchain="D:\CodeCanvas_2406041022\riscv\gnu-toolchain" --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -T"D:/FDMTEK_RV335/dsp_fastrts/example/abs_sp_cv/system/link_riscv.ld" -L"D:\FDMTEK_RV335\dsp_fastrts\example\abs_sp_cv\src\include" -LD:\CodeCanvas_2406041022\riscv\gnu-toolchain\riscv32-unknown-elf\lib -L"D:\FDMTEK_RV335\dsp_fastrts\example\abs_sp_cv\lib" -lrv335dspmath -o "abs_sp_cv.elf"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ELF_FILE_OUTPUTS)$(OBJECT_FILE) abs_sp_cv.elf
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
