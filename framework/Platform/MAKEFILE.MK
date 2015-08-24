all:
	make -fMAKEFILE.MK -C$(base_PATH)/Platform/Src
	make -fMAKEFILE.MK -C$(base_PATH)/Platform/Src/Linux

clean:
	make -fMAKEFILE.MK -C$(base_PATH)/Platform/Src       clean
	make -fMAKEFILE.MK -C$(base_PATH)/Platform/Src/Linux clean
.PHONY: all clean
