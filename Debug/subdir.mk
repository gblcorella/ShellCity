################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../get_path.c \
../get_path_main.c \
../main.c \
../sh.c 

OBJS += \
./get_path.o \
./get_path_main.o \
./main.o \
./sh.o 

C_DEPS += \
./get_path.d \
./get_path_main.d \
./main.d \
./sh.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


