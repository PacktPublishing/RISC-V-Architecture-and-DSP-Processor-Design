################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/include/abs_SP_CV.s 

OBJECT_FILE += \
./src/include/abs_SP_CV.o 


# Each subdirectory must supply rules for building sources it contributes
src/include/%.o: ../src/include/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Assembler'
	"D:\IDE\240223\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -ID:\IDE\240223\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include -ID:\IDE\240223\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include-fixed -ID:\IDE\240223\riscv\gnu-toolchain\riscv32-unknown-elf\include -ID:\IDE\240223\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0 -ID:\IDE\240223\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0\riscv32-unknown-elf -I"F:\SpringCore\SpringCore\RV335_DEBUG\ShengTai\DSP_Math\example\abs_sp_cv\src\include" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


