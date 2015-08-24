LIBA    = $(lib_PATH)/LibLog4cpp.a  
COMPILE = $(CC)  $(CFLAGS) $(Inc) -c  
SRCS    = $(wildcard *.cpp)  
OBJS    = $(patsubst %.cpp,%.o,$(SRCS))  
all:$(OBJS)
%.o:%.cpp 
	$(COMPILE) $< -o $@ 
	$(AR) cqs $(LIBA) $@  
clean:
	@echo + rm $(OBJS) 
	rm -f $(OBJS)  
.PHONY:clean all
