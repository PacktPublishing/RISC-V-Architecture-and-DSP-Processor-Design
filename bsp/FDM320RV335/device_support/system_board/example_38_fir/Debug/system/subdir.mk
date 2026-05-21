################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../system/fdm_start.s 

C_SRCS += \
../system/RV335_Adc.c \
../system/RV335_GlobalVariableDefs.c \
../system/RV335_SysCtrl.c \
../system/intexc.c 

S_UPPER_SRCS += \
../system/trap.S 

OBJECT_FILE += \
./system/RV335_Adc.o \
./system/RV335_GlobalVariableDefs.o \
./system/RV335_SysCtrl.o \
./system/fdm_start.o \
./system/intexc.o \
./system/trap.o 


# Each subdirectory must supply rules for building sources it contributes
system/%.o: ../system/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Compiler'
	"D:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -mcmodel=medany -mcpu=springcore-rv32 -mllvm -enable-misched -mllvm -misched-topdown=true -ID:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include -I"D:\IDE\240223\240223\workspace\example_38_fir\routines\include" -ID:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include-fixed -ID:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\gnu-toolchain\riscv32-unknown-elf\include -ID:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0 -ID:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0\riscv32-unknown-elf -I"D:\IDE\240223\240223\workspace\example_38_fir\include" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/%.o: ../system/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Assembler'
	"D:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -ID:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include -ID:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include-fixed -ID:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\gnu-toolchain\riscv32-unknown-elf\include -ID:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0 -ID:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0\riscv32-unknown-elf -I"D:\IDE\240223\240223\workspace\example_38_fir\include" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

system/%.o: ../system/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Assembler'
	"D:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -ID:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include -ID:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\10.2.0\include-fixed -ID:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\gnu-toolchain\riscv32-unknown-elf\include -ID:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0 -ID:\IDE\CodeCanvas_2403151523\CodeCanvas_2403151523\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\10.2.0\riscv32-unknown-elf -I"D:\IDE\240223\240223\workspace\example_38_fir\include" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


