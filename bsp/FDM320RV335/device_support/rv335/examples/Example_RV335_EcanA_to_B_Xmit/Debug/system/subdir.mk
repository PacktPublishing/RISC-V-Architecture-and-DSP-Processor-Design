################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../system/RV335_usDelay.s \
../system/fdm_start.s 

C_SRCS += \
../system/RV335_Adc.c \
../system/RV335_GlobalVariableDefs.c \
../system/RV335_SysCtrl.c \
../system/intexc.c 

S_UPPER_SRCS += \
../system/trap.S 

OBJECT_FILE += \
./system/RV335_Adc.o \
./system/RV335_GlobalVariableDefs.o \
./system/RV335_SysCtrl.o \
./system/RV335_usDelay.o \
./system/fdm_start.o \
./system/intexc.o \
./system/trap.o 


# Each subdirectory must supply rules for building sources it contributes
system/%.o: ../system/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Compiler'
	"D:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -mcmodel=medany -ID:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include -ID:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include-fixed -ID:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\gnu-toolchain\riscv32-unknown-elf\include -ID:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0 -ID:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0\riscv32-unknown-elf -I"D:\001_project\001_RISC-V\016_IDE_workspace\rv335_v0.5\Example_RV335_EcanA_to_B_Xmit\include" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/%.o: ../system/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Assembler'
	"D:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -ID:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include -ID:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include-fixed -ID:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\gnu-toolchain\riscv32-unknown-elf\include -ID:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0 -ID:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0\riscv32-unknown-elf -I"D:\001_project\001_RISC-V\016_IDE_workspace\rv335_v0.5\Example_RV335_EcanA_to_B_Xmit\include" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/%.o: ../system/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Assembler'
	"D:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -ID:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include -ID:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include-fixed -ID:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\gnu-toolchain\riscv32-unknown-elf\include -ID:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0 -ID:\001_project\001_RISC-V\013_subsystem\001_drivelib\CodeCanvas_202402281348\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0\riscv32-unknown-elf -I"D:\001_project\001_RISC-V\016_IDE_workspace\rv335_v0.5\Example_RV335_EcanA_to_B_Xmit\include" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


