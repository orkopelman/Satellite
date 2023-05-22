CXX = g++
CC = $(CXX)

CPPFLAGS = -I$(INC_DIR)  -I$(CSP_INCLUDES) -I$(CSP_INCLUDES2) -I$(SPAM_INCLUDE)
CPPFLAGS += -MMD -MP
# CPPFLAGS += -DDEBUG
# c language
# CFLAGS = -g3 -pthread

# c++ language
CXXFLAGS = -std=c++17
CXXFLAGS += -pthread
CXXFLAGS+= -Wall -pedantic
#CXXFLAGS+= -Werror
CXXFLAGS+= -g3

LDFLAGS = -pthread
LDFLAGS += -L$(CSP_BUILD_DIR) $(CSP_SO)

CSP_SO =  -Wl,-rpath,$(CSP_BUILD_DIR) $(CSP_BUILD_DIR)/liblibcsp.so
LDLIBS =  -l:liblibcsp.so
LDLIBS +=  -lsqlite3

BASE_DIR = ../..
CSP_BASE_DIR = $(BASE_DIR)/libcsp
CSP_BUILD_DIR = $(CSP_BASE_DIR)/build
CSP_INCLUDES = $(CSP_BASE_DIR)/include
CSP_INCLUDES2 = $(CSP_BUILD_DIR)

SPAM_INCLUDE = $(BASE_DIR)/SPAM

SRC_DIR= $(BASE_DIR)/src/
INC_DIR= $(BASE_DIR)/inc/


TARGET = utest

OBJS = utest.o $(SRC_DIR)housekeeping_data.o $(SRC_DIR)collector_intermediate.o $(SRC_DIR)collectors.o $(SRC_DIR)sqlite_database.o

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