#CC=arm-linux-gnueabihf-g++ -mfpu=neon -mtune=cortex-a9 
CC=g++
base_PATH=$(shell pwd)
lib_PATH =$(base_PATH)/Lib
Inc      = -I$(base_PATH)  
bin_PATH =$(base_PATH)/Bin
AR = ar 
#CFLAGS = -O2

export CC
export base_PATH
export lib_PATH
export bin_PATH
export AR
export Inc
