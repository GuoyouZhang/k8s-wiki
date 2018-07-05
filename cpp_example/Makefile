CC	= g++
CFLAGS	= -g3 -pthread -std=c++11
GCOV	= -fprofile-arcs -ftest-coverage
INCS	= -I.
LIBS	=
TARGET	= parser
SRCS	= $(wildcard ./*.cc)

OBJS	= $(SRCS:%.cc=%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(GCOV) -o $@ $^ $(LIBS)

%.o: %.cc
	$(CC) $(CFLAGS) $(GCOV) -c $< -o $@

.PHONY: clean

clean:
	rm $(TARGET) *.o *.gcno
