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
CXXFLAGS+= -Wextra
CXXFLAGS+= -Werror
CXXFLAGS+= -g3

LDFLAGS = -pthread
LDFLAGS += -L$(CSP_BUILD_DIR) $(CSP_SO)

CSP_SO =  -Wl,-rpath,$(CSP_BUILD_DIR) $(CSP_BUILD_DIR)/liblibcsp.so
LDLIBS =  -l:liblibcsp.so

BASE_DIR = ../..
CSP_BASE_DIR = $(BASE_DIR)/libcsp
CSP_BUILD_DIR = $(CSP_BASE_DIR)/build
CSP_INCLUDES = $(CSP_BASE_DIR)/include
CSP_INCLUDES2 = $(CSP_BUILD_DIR)

SPAM_INCLUDE = $(BASE_DIR)/SPAM

SRC_DIR= $(BASE_DIR)/src/
INC_DIR= $(BASE_DIR)/inc/


TARGET = utest

OBJS = utest.o $(SRC_DIR)csp_boot.o $(SRC_DIR)muhsat_command_kitchen.o $(SRC_DIR)muhsat_events_kitchen.o $(SRC_DIR)muhsat_handlers_kitchen.o $(SRC_DIR)muhsat_modes_kitchen.o $(SRC_DIR)muhsat_protocol_kitchen.o $(SRC_DIR)muhsat_router_kitchen.o $(SRC_DIR)muhsat_trackers_kitchen.o $(SRC_DIR)housekeeping_data.o 
OBJS += $(SRC_DIR)collector_intermediate.o $(SRC_DIR)collectors.o $(SRC_DIR)sqlite_database.o
OBJS += $(SRC_DIR)intermediate_trackable_event.o $(SRC_DIR)intermediate_tracker.o 

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