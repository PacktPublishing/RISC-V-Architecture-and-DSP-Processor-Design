################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/include/CFFT_f32_unpack.s \
../src/include/RFFT_f32.s \
../src/include/RFFT_f32_brev.s \
../src/include/RFFT_f32_mag.s \
../src/include/RFFT_f32_phase.s \
../src/include/RFFT_f32_sincostable.s \
../src/include/RFFT_f32_win.s 

OBJECT_FILE += \
./src/include/CFFT_f32_unpack.o \
./src/include/RFFT_f32.o \
./src/include/RFFT_f32_brev.o \
./src/include/RFFT_f32_mag.o \
./src/include/RFFT_f32_phase.o \
./src/include/RFFT_f32_sincostable.o \
./src/include/RFFT_f32_win.o 


# Each subdirectory must supply rules for building sources it contributes
src/include/%.o: ../src/include/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Assembler'
	"C:\2204\IDE\231130\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -IC:\2204\IDE\231130\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include -IC:\2204\IDE\231130\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include-fixed -IC:\2204\IDE\231130\riscv\gnu-toolchain\riscv32-unknown-elf\include -IC:\2204\IDE\231130\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0 -IC:\2204\IDE\231130\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0\riscv32-unknown-elf -I"C:\WorkSpace\gaoyuxin_workspace\2204\2204_benchmark\FPU32_RFFT\src\include" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


