################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/include/CFFT32_win.s \
../src/include/FFT32_brev.s \
../src/include/FFT32_calc.s \
../src/include/FFT32_init.s \
../src/include/FFT32_init2.s \
../src/include/FFT32_izero.s \
../src/include/FFT32_mag.s \
../src/include/RFFT32_split.s \
../src/include/RFFT32_win.s 

OBJECT_FILE += \
./src/include/CFFT32_win.o \
./src/include/FFT32_brev.o \
./src/include/FFT32_calc.o \
./src/include/FFT32_init.o \
./src/include/FFT32_init2.o \
./src/include/FFT32_izero.o \
./src/include/FFT32_mag.o \
./src/include/RFFT32_split.o \
./src/include/RFFT32_win.o 


# Each subdirectory must supply rules for building sources it contributes
src/include/%.o: ../src/include/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Assembler'
	"C:\2204\IDE\231130\riscv\llvm\bin\clang" -mllvm --rptb-enable-cb -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -IC:\2204\IDE\231130\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include -IC:\2204\IDE\231130\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include-fixed -IC:\2204\IDE\231130\riscv\gnu-toolchain\riscv32-unknown-elf\include -IC:\2204\IDE\231130\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0 -IC:\2204\IDE\231130\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0\riscv32-unknown-elf -I"C:\WorkSpace\gaoyuxin_workspace\2204\2204_benchmark\FixedPoint_FFT\src\include" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


