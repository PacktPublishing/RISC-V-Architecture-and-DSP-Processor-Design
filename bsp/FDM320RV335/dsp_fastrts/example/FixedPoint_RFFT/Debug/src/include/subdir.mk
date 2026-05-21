################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/include/FFT32_brev.s \
../src/include/FFT32_brev2.s \
../src/include/FFT32_calc.s \
../src/include/FFT32_init.s \
../src/include/FFT32_init2.s \
../src/include/FFT32_mag.s \
../src/include/RFFT32_split.s \
../src/include/RFFT32_win.s \
../src/include/RFFT_f32_brev.s 

OBJECT_FILE += \
./src/include/FFT32_brev.o \
./src/include/FFT32_brev2.o \
./src/include/FFT32_calc.o \
./src/include/FFT32_init.o \
./src/include/FFT32_init2.o \
./src/include/FFT32_mag.o \
./src/include/RFFT32_split.o \
./src/include/RFFT32_win.o \
./src/include/RFFT_f32_brev.o 


# Each subdirectory must supply rules for building sources it contributes
src/include/%.o: ../src/include/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Assembler'
	"D:\IDE\240223\riscv\llvm\bin\clang" -mllvm --rptb-enable-cb -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -ID:\IDE\240223\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include -ID:\IDE\240223\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include-fixed -ID:\IDE\240223\riscv\gnu-toolchain\riscv32-unknown-elf\include -ID:\IDE\240223\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0 -ID:\IDE\240223\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0\riscv32-unknown-elf -I"F:\SpringCore\SpringCore\RV335_DEBUG\ShengTai\DSP_Math\example\FixedPoint_RFFT\src\include" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


