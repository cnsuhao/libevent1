LIBA    = $(lib_PATH)/LibXml.a  
COMPILE = $(CC)  $(CFLAGS) $(Inc) -c  
SRCS    = $(wildcard *.cpp)  
OBJS    = $(patsubst %.cpp,%.o,$(SRCS))  
app    := TinyTest
all:$(OBJS) app
%.o:%.cpp 
	$(COMPILE) $< -o $@ 
app:
	$(CC) $(OBJS)  -lpthread -lrt -ldl  -Xlinker "-(" $(LIBA)  -Xlinker "-)"   -o $(app)  
clean:
	@echo + rm $(OBJS) 
	rm -f $(OBJS)  
.PHONY:clean all
