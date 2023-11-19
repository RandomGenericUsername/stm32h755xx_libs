
CC := arm-none-eabi-gcc
CXX := arm-none-eabi-g++
TEST_CC := gcc
TEST_CXX := g++
OBJCOPY := arm-none-eabi-objcopy

BUILD_PATH := ../../Build
TARGET := ../../Build/m4/stm32h755xx_libs_m4.elf
TEST_BUILD_PATH := ../../Build/Tests
TEST_TARGET := ../../Build/Tests/stm32h755xx_libs_test.elf
CORE_REL_PATH := ../../Core
STARTUP_REL_PATH := ../../Startup

# Define include directories
INC_DIRS := $(shell find ../../Core -type d -name .svn -prune -o -type d -print)
INC_FLAGS := $(addprefix -I, $(INC_DIRS))
C_FLAGS_DEF := $(addprefix -I, $(INC_DIRS)) -mthumb -mcpu=cortex-m4 -specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -g3 -O0 -ffunction-sections -fdata-sections -fexceptions -Wall -fstack-usage
CXX_FLAGS_DEF := $(addprefix -I, $(INC_DIRS)) -mthumb -mcpu=cortex-m4 -specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -g3 -O0 -ffunction-sections -fdata-sections -fexceptions -Wall -fstack-usage -fno-rtti -fno-use-cxa-atexit
TEST_C_FLAGS_DEF := -std=gnu11 -g3 -O0 -Wall $(addprefix -I, $(INC_DIRS))
TEST_CXX_FLAGS_DEF := -std=c++20 -g3 -O0 -Wall $(addprefix -I, $(INC_DIRS))
LINKER_FLAGS := -Wl,-Map=$(TARGET:.elf=.map),--cref -mthumb -mcpu=cortex-m4 -specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group -Wl,--print-memory-usage

CXX_SOURCES_CORE := $(shell find ../../Core -name '*.cpp'  -not -path "../../Core/m7/*")
C_SOURCES_CORE := $(shell find ../../Core -name '*.c'  -not -path "../../Core/m7/*")
TEST_CXX_SOURCES_CORE := $(shell find ../../Tests -name '*.cpp') $(shell find ../../Core -name '*.cpp'  -not -path "../../Core/m4/*" -not -path "../../Core/m7/*"  -not -name "sysmem.c" -not -name "syscalls.c")
TEST_C_SOURCES_CORE := $(shell find ../../Tests -name '*.c') $(shell find ../../Core -name '*.c'  -not -path "../../Core/m4/*" -not -path "../../Core/m7/*"  -not -name "sysmem.c" -not -name "syscalls.c")
STARTUP_SCRIPT_PATH := ../../Startup/startup_stm32h755xx.s
LINKER_SCRIPT_PATH := ../../Startup/stm32h755xx_flash_CM4.ld
OBJ_DIR := ../../Build/m4
TEST_OBJ_DIR := ../../Build/Tests
CXX_OBJECTS := $(CXX_SOURCES_CORE:${CORE_REL_PATH}/%.cpp=$(OBJ_DIR)/%.o)
C_OBJECTS := $(C_SOURCES_CORE:${CORE_REL_PATH}/%.c=$(OBJ_DIR)/%.o)
TEST_CXX_OBJECTS := $(TEST_CXX_SOURCES_CORE:../..//%.cpp=../../Build/Tests/%.o)
TEST_C_OBJECTS := $(TEST_C_SOURCES_CORE:../..//%.c=../../Build/Tests/%.o)
ASM_OBJECTS := $(STARTUP_SCRIPT_PATH:${STARTUP_REL_PATH}/%.s=$(OBJ_DIR)/Startup/%.o)
DEPS := $(CXX_OBJECTS:.o=.d) $(C_OBJECTS:.o=.d) $(ASM_OBJECTS:.o=.d)

-include $(DEPS)

all: clean build

build: $(TARGET) $(TARGET:.elf=.bin)

flash: all
	st-flash --reset write $(TARGET:.elf=.bin) 0x08000000

all_test: clean_test build_test

build_test: $(TEST_TARGET)

run_test: build_test
	@./$(TEST_TARGET)

$(TARGET): $(CXX_OBJECTS) $(C_OBJECTS) $(ASM_OBJECTS)
	$(CXX) -T $(LINKER_SCRIPT_PATH) $^ -o $@ $(LINKER_FLAGS)
	@echo 'Finished building target: $@'
	@echo ' '

# Rule to convert .elf to .bin
$(TARGET:.elf=.bin): $(TARGET)
	$(OBJCOPY) -O binary $< $@
	@echo 'Finished building binary: $@'
	@echo ' '

$(TEST_TARGET): $(TEST_CXX_OBJECTS) $(TEST_C_OBJECTS)
	@mkdir -p $(dir $@)  
	$(TEST_CXX) $^ -o $@ $(TEST_CXX_FLAGS_DEF)
	@echo 'Finished building test target: $@'
	@echo ' '

$(OBJ_DIR)/%.o: ../../Core/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -std=gnu++20 -c $< $(CXX_FLAGS_DEF) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@"

$(OBJ_DIR)/%.o: ../../Core/%.c
	@mkdir -p $(dir $@)
	$(CC) -c -std=gnu11 $< $(C_FLAGS_DEF) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@"

$(OBJ_DIR)/Startup/%.o: ../../Startup/%.s
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: ../../Tests/%.cpp
	@mkdir -p $(dir $@)
	$(TEST_CXX) -c $< $(TEST_CXX_FLAGS_DEF) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@"

$(TEST_OBJ_DIR)/%.o: ../../Tests/%.c
	@mkdir -p $(dir $@)
	$(TEST_CC) -c $< $(TEST_C_FLAGS_DEF) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@"

$(TEST_OBJ_DIR)/%.o: ../../Core/%.cpp
	@mkdir -p $(dir $@)
	$(TEST_CXX) -c $< $(TEST_CXX_FLAGS_DEF) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@"

$(TEST_OBJ_DIR)/%.o: ../../Core/%.c
	@mkdir -p $(dir $@)
	$(TEST_CC) -c $< $(TEST_C_FLAGS_DEF) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@"

.PHONY: clean clean_test

clean:
	rm -rf $(OBJ_DIR)/* $(TARGET)

clean_test:
	rm -rf $(TEST_OBJ_DIR)/* $(TEST_TARGET)

