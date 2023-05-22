CXX = g++
CC = $(CXX)

CPPFLAGS = -I $(INC_DIR)
CPPFLAGS += -MMD -MP
# CPPFLAGS += -DDEBUG
# c language
CFLAGS = -g3

# c++ language
CXXFLAGS = -std=c++17
CXXFLAGS+= -Wall -pedantic
CXXFLAGS+= -Wextra
CXXFLAGS+= -Werror
CXXFLAGS+= -g3

LDFLAGS = -pthread
LDLIBS =

SRC_DIR= ../../src/
INC_DIR= ../../inc/

TARGET = utest
# ! Fill only this:
OBJS = utest.o

all: $(TARGET)

$(TARGET): $(OBJS)

check: all
	./$(TARGET) -v

vala: all
	valgrind --leak-check=yes --track-origins=yes -s ./$(TARGET)

DEPENDS= $(OBJS:%.o=%.d)

clean:
	@$(RM) $(TARGET) $(OBJS) $(DEPENDS)

.PHONY: all check clean DEPENDS

-include $(DEPENDS)