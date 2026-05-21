################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/include/iir16.s \
../src/include/iir32.s 

OBJECT_FILE += \
./src/include/iir16.o \
./src/include/iir32.o 


# Each subdirectory must supply rules for building sources it contributes
src/include/%.o: ../src/include/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Assembler'
	"C:\2204\IDE\240223\riscv\llvm\bin\clang" -mllvm --rptb-enable-cb -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -IC:\2204\IDE\240223\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include -IC:\2204\IDE\240223\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include-fixed -IC:\2204\IDE\240223\riscv\gnu-toolchain\riscv32-unknown-elf\include -IC:\2204\IDE\240223\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0 -IC:\2204\IDE\240223\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0\riscv32-unknown-elf -I"C:\WorkSpace\gaoyuxin_workspace\SpringCore\ShengTai\relase\DSP_Math\example\FixedPoint_IIR16\src\include" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


