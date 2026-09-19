CFLAGS = -g -Wall -Wshadow 
GCC = gcc $(CFLAGS)
LIBS = -lm
SRCS = main.c 
# SRCS += bmpequalize.c bmphistogram.c bmpchecker.c bmpmix.c bmpgray.c
TARGET = image
OBJS = $(SRCS:%.c=%.o)
VALGRIND = valgrind --leak-check=full --tool=memcheck --verbose --log-file

$(TARGET): $(OBJS)
	$(GCC) $(OBJS) -o $@ $(LIBS)

.c.o: 
	$(GCC) -c $*.c 

test: $(TARGET)
	rm -f -r outputs/*
	mkdir outputs
	./image inputs/colorchecker.bmp mix outputs/colormix.bmp
	

clean:
	rm -f *.o a.out $(TARGET) *log
	rm -f -r outputs *~ 
