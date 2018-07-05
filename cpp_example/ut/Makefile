ver	= debug
CC	= g++
CFLAGS	= -g3 -pthread -std=c++11

GMOCK_DIR=/home/mininet/googletest-master/googlemock
GTEST_DIR=/home/mininet/googletest-master/googletest

INCS	= -I. -I../ \
	  -I$(GMOCK_DIR)/include/ \
	  -I$(GTEST_DIR)/include/

LIBS	= $(GMOCK_DIR)/gtest/libgtest.a \
	  $(GMOCK_DIR)/libgmock.a

TARGET	= ut

SRCS	= ../MsgQueueImp.cc \
	  ../StateMachineImp.cc \
	  ./ut_state_machine.cc \
	  ./run_all.cc

OBJS	= $(SRCS:%.cc=%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(GCOV) -o $@ $^ $(LIBS)

%.o: %.cc
	$(CC) $(CFLAGS) $(GCOV) $(INCS) -c $< -o $@

.PHONY: clean

clean:
	rm $(TARGET) *.o 
