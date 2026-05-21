################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/mpy_SP_CVxCVC.s 

C_SRCS += \
../src/main.c 

OBJECT_FILE += \
./src/main.o \
./src/mpy_SP_CVxCVC.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Compiler'
	"D:\CodeCanvas_2406041022\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -mcmodel=medany -mcpu=springcore-rv32 -mllvm -enable-misched -mllvm -misched-topdown=true -ID:\CodeCanvas_2406041022\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include -I"D:\FDMTEK_RV335\dsp_fastrts\example\mpy_SP_CVxCVC\src\include" -ID:\CodeCanvas_2406041022\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include-fixed -ID:\CodeCanvas_2406041022\riscv\gnu-toolchain\riscv32-unknown-elf\include -ID:\CodeCanvas_2406041022\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0 -ID:\CodeCanvas_2406041022\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0\riscv32-unknown-elf -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Assembler'
	"D:\CodeCanvas_2406041022\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


