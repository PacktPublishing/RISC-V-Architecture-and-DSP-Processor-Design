################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := D:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas\CodeCanvas\build-utils\clean

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
all: Example_RV335_Sci_FFDLB_int.elf

# Tool invocations
Example_RV335_Sci_FFDLB_int.elf: $(OBJECT_FILE)
	@echo 'Building target: $@'
	@echo 'Invoking: RISCV linker'
	"D:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas\CodeCanvas\riscv\llvm\bin\clang" $(OBJECT_FILE)  --gcc-toolchain="D:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas\CodeCanvas\riscv\gnu-toolchain" --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -T"D:/001_project/001_RISC-V/016_IDE_workspace/rv335_v0.3/Example_RV335_Sci_FFDLB_int/system/link_riscv.ld" -LD:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas\CodeCanvas\riscv\gnu-toolchain\riscv32-unknown-elf\lib -o "Example_RV335_Sci_FFDLB_int.elf"
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ELF_FILE_OUTPUTS)$(OBJECT_FILE) Example_RV335_Sci_FFDLB_int.elf
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
