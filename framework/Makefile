include Rules.make
libs=$(lib_PATH)/LibXml.a  $(lib_PATH)/LibPlatform.a 
COMPILE = $(CC)  $(CFLAGS) $(Inc) -c 
SRCS := $(wildcard *.cpp)  
OBJS := $(patsubst %.cpp,%.o,$(SRCS))  
app  := UartControl
all: LibPlatform Tinyxml Test
LibPlatform:
	make -fMAKEFILE.MK -C$(base_PATH)/Platform
Tinyxml:
	make -fMAKEFILE.MK -C$(base_PATH)/Tinyxml
Test: Tinyxmltest
Tinyxmltest:
	make -fMAKEFILE.MK -C$(base_PATH)/Tinyxmltest
#exe:$(OBJS) app
#%.o:%.cpp 
#	$(COMPILE) $< -o $@ 
#app:
#	$(CC) $(OBJS)  -lpthread -lrt -ldl  -Xlinker "-(" $(libs) $(lib_PATH1)/libsqlite3.a  -Xlinker "-)"   -o $(app)  
clean: 
	make -fMAKEFILE.MK -C$(base_PATH)/Platform      clean
	make -fMAKEFILE.MK -C$(base_PATH)/Tinyxml       clean
	make -fMAKEFILE.MK -C$(base_PATH)/Tinyxmltest   clean
	@echo + rm $(lib_PATH)/*.a
	-rm $(lib_PATH)/*.a
	@echo + rm $(OBJS) 
	rm -f $(OBJS) $(app)
.PHONY: exe Test  clean  LibPlatform Tinyxml Tinyxmltest
