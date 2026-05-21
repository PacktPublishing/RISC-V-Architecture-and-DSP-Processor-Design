################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/include/FFT32_init.s \
../src/include/fir32_opt.s 

OBJECT_FILE += \
./src/include/FFT32_init.o \
./src/include/fir32_opt.o 


# Each subdirectory must supply rules for building sources it contributes
src/include/%.o: ../src/include/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Assembler'
	"C:\2204\IDE\231130\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -IC:\2204\IDE\231130\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include -IC:\2204\IDE\231130\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include-fixed -IC:\2204\IDE\231130\riscv\gnu-toolchain\riscv32-unknown-elf\include -IC:\2204\IDE\231130\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0 -IC:\2204\IDE\231130\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0\riscv32-unknown-elf -I"C:\WorkSpace\gaoyuxin_workspace\SpringCore\ÉúÌ¬¿â\relase\DSP_Math\example\FixedPoint_Fir32\src\include" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


