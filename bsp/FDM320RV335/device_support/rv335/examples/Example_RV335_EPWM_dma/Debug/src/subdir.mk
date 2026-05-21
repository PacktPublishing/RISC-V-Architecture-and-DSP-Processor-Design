################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c 

OBJECT_FILE += \
./src/main.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Compiler'
	"C:\Users\xrz\Desktop\240117\240117\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -mcmodel=medany -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include-fixed -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0 -IC:\Users\xrz\Desktop\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0\riscv32-unknown-elf -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


