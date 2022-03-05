PROG_NAME ?= binary_csv
DEBUG ?= n
VERBOSE ?= n

OBJ_DIR = .obj

ifeq ($(VERBOSE),y)
CC = g++
LD = g++
else
CC = @g++
LD = @g++
endif

INC_DIRS += ./lib
INC_DIRS += ./test/Unity/src

CPPFLAGS += $(addprefix -I, $(INC_DIRS))
CPPFLAGS += -O0
CPPFLAGS += -Wall
CPPFLAGS += -Werror
ifeq ($(DEBUG),y)
CPPFLAGS += -g
endif

PROG_SRCS := $(shell find src/ -name "*.cpp")
LIB_SRCS := $(shell find lib/ -name "*.cpp")
TEST_SRCS := $(shell find test/ -name "*.cpp")

PROG_OBJS := $(addprefix $(OBJ_DIR)/, $(patsubst %.cpp,%.o,$(PROG_SRCS)))
LIB_OBJS := $(addprefix $(OBJ_DIR)/, $(patsubst %.cpp,%.o,$(LIB_SRCS)))
TEST_OBJS := $(addprefix $(OBJ_DIR)/, $(patsubst %.cpp,%.o,$(TEST_SRCS)))
TEST_NAME := $(addprefix $(OBJ_DIR)/, test_runner)

DEPS := $(shell find . -name "*.h")

all: $(PROG_NAME)

$(PROG_NAME): $(LIB_OBJS) $(PROG_OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

test: $(TEST_NAME)
	@./$(TEST_NAME)

$(TEST_NAME): $(LIB_OBJS) $(TEST_OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.cpp $(DEPS)
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CPPFLAGS)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -f $(PROG_NAME)
	@rm -f $(TEST_NAME)

.PHONY: test
