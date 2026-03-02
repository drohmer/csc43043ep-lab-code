# ====== Basic Configuration ======
TARGET      ?= project
BUILD_DIR   ?= build
SRC_DIRS    ?= src
CXX         ?= g++
CXXSTD      ?= c++17

# Header include directories
INC_DIRS    := . src

# Preprocessor definitions (-D)
DEFINES     := SOLUTION

# ====== Sources / Objects / Dependencies ======
# Find .cpp and .c files
SRCS := $(shell find $(SRC_DIRS) -type f \( -name '*.cpp' -o -name '*.c' \))
# Put object files under build/ while keeping folder structure
OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS)))
DEPS := $(OBJS:.o=.d)


# ====== Compiler/Linker Flags ======
CPPFLAGS += $(addprefix -I,$(INC_DIRS)) \
            $(addprefix -D,$(DEFINES)) -MMD -MP
CXXFLAGS += -std=$(CXXSTD) -g -O2 -Wall -Wextra -Wfatal-errors \
            -Wno-sign-compare -Wno-type-limits -Wno-pragmas
LDFLAGS  +=
LDLIBS   += 

# System-specific libraries
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
  LDLIBS += -ldl -lm
endif
ifeq ($(UNAME_S),Darwin)
  LDLIBS += -lm
endif

# ====== Rules ======
.PHONY: all clean run
.DEFAULT_GOAL := all
.DELETE_ON_ERROR:

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Link  $@"
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LDLIBS)

# Compile C++ sources
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "C++   $<"
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Compile C sources
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "C     $<"
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	$(RM) -r $(BUILD_DIR) $(TARGET) $(DEPS)

-include $(DEPS)
