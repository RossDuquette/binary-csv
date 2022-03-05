PROG_NAME ?= csv_compressor
DEBUG ?= n
VERBOSE ?= n

OBJ_DIR = .obj

SRCS += $(shell find src/ -name "*.cpp")

ifeq ($(VERBOSE),y)
CC = g++
LD = g++
else
CC = @echo '   ' CC $@; g++
LD = @echo '   ' LD $@; g++
endif

INC_DIRS += ./src

CPPFLAGS += $(addprefix -I, $(INC_DIRS))
CPPFLAGS += -O0
CPPFLAGS += -Wall
CPPFLAGS += -Werror
ifeq ($(DEBUG),y)
CPPFLAGS += -g
endif

DEPS := $(shell find . -name "*.h")
OBJS := $(addprefix $(OBJ_DIR)/, $(patsubst %.cpp,%.o,$(SRCS)))

all: $(PROG_NAME)

$(PROG_NAME): $(OBJS)
	$(LD) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.cpp $(DEPS)
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CPPFLAGS)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -f $(PROG_NAME)
