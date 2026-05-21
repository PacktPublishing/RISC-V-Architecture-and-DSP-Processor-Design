################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/TIMER0.c 

OBJECT_FILE += \
./src/TIMER0.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Compiler'
	"D:\IDE\CodeCanvas_2403151523\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -mcmodel=medany -mcpu=springcore-rv32 -mllvm -enable-misched -mllvm -misched-topdown=true -ID:\IDE\CodeCanvas_2403151523\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include -ID:\IDE\CodeCanvas_2403151523\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include-fixed -ID:\IDE\CodeCanvas_2403151523\riscv\gnu-toolchain\riscv32-unknown-elf\include -ID:\IDE\CodeCanvas_2403151523\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0 -ID:\IDE\CodeCanvas_2403151523\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0\riscv32-unknown-elf -I"F:\SpringCore\SpringCore\FDMTEK_RV335\Hello_LED\include" -I"F:\SpringCore\SpringCore\FDMTEK_RV335\Hello_LED\routines\include" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


