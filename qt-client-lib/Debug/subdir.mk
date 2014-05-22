################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../qt_analyse.cpp \
../qt_datamgr.cpp \
../qt_mgrdev.cpp \
../qt_netdev.cpp \
../qt_network.cpp \
../qt_nibpmgr.cpp \
../simulator_client.cpp 

OBJS += \
./qt_analyse.o \
./qt_datamgr.o \
./qt_mgrdev.o \
./qt_netdev.o \
./qt_network.o \
./qt_nibpmgr.o \
./simulator_client.o 

CPP_DEPS += \
./qt_analyse.d \
./qt_datamgr.d \
./qt_mgrdev.d \
./qt_netdev.d \
./qt_network.d \
./qt_nibpmgr.d \
./simulator_client.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/root/workdir/qt-client-lib/thread_task -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


