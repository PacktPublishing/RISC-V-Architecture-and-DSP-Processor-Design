################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/RV335_ECap_apwm.c 

OBJECT_FILE += \
./src/RV335_ECap_apwm.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: RISCV Compiler'
	"G:\Awork_FDM\007-Project\2204\240117\240117\riscv\llvm\bin\clang" -g -c --target=riscv32-unknown-elf -march=rv32imfc_xsc -mabi=ilp32f -mcmodel=medany -IG:\Awork_FDM\007-Project\2204\240117\240117\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include -IG:\Awork_FDM\007-Project\2204\240117\240117\riscv\gnu-toolchain\lib\gcc\riscv32-unknown-elf\12.2.0\include-fixed -IG:\Awork_FDM\007-Project\2204\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include -IG:\Awork_FDM\007-Project\2204\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0 -IG:\Awork_FDM\007-Project\2204\240117\240117\riscv\gnu-toolchain\riscv32-unknown-elf\include\c++\12.2.0\riscv32-unknown-elf -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


