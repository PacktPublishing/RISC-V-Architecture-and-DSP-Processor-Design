################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/include/atan_f32.s 

OBJECT_FILE += \
./src/include/atan_f32.o 


# Each subdirectory must supply rules for building sources it contributes
src/include/%.o: ../src/include/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Assembler'
	"C:\2204\IDE\231130\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -IC:\2204\IDE\231130\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include -IC:\2204\IDE\231130\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include-fixed -IC:\2204\IDE\231130\riscv\gnu-toolchain\riscv32-unknown-elf\include -IC:\2204\IDE\231130\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0 -IC:\2204\IDE\231130\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0\riscv32-unknown-elf -I"C:\WorkSpace\gaoyuxin_workspace\2204\2204_benchmark\FPU32_atan\src\include" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


