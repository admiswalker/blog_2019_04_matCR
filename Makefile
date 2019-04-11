# name of generate file
TARGET = a.out

# options
CFLAGS  = -std=gnu++0x
CFLAGS += -Wall
CFLAGS += -O3

# objects
OBJS = main.cpp

$(TARGET): $(OBJS)
	$(CXX) main.cpp $(CFLAGS)

.PHONY: clean
clean:
	-rm -rf $(TARGET)
