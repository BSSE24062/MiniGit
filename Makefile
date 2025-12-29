# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -w -I. -MMD -MP

# Directories
BUILD_DIR := build
INCLUDE_DIR := .

# Source files
MAIN_SRC := main.cpp
TEST_SRC := test.cpp
S_TEST_SRC := test_s.cpp
H_TEST_SRC := test_h.cpp
ALL_CPP := $(wildcard *.cpp)
COMMON_SRC := $(filter-out $(MAIN_SRC) $(TEST_SRC) $(S_TEST_SRC) $(H_TEST_SRC), $(ALL_CPP))

# Object files
COMMON_OBJ := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(COMMON_SRC))
MAIN_OBJ := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(MAIN_SRC))
TEST_OBJ := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(TEST_SRC))
S_TEST_OBJ := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(S_TEST_SRC))
H_TEST_OBJ := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(H_TEST_SRC))

# Binaries
RUN_BIN := $(BUILD_DIR)/run.out
TEST_BIN := $(BUILD_DIR)/test.out
S_TEST_BIN := $(BUILD_DIR)/s_test.out
H_TEST_BIN := $(BUILD_DIR)/h_test.out

# Dependency files
DEPS := $(COMMON_OBJ:.o=.d) $(MAIN_OBJ:.o=.d) $(TEST_OBJ:.o=.d) $(H_TEST_OBJ:.o=.d)

# Default target
all: $(TEST_BIN)

# Ensure build dir exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Generic compilation rule with dependency generation
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@$(CXX) -c -I$(INCLUDE_DIR) $(CXXFLAGS) -MF $(patsubst %.o,%.d,$@) $< -o $@

# Link targets
$(RUN_BIN): $(MAIN_OBJ) $(COMMON_OBJ)
	$(CXX) $^ -o $@

$(TEST_BIN): $(TEST_OBJ) $(COMMON_OBJ)
	$(CXX) $^ -o $@

$(S_TEST_BIN): $(S_TEST_OBJ) $(COMMON_OBJ)
	$(CXX) $^ -o $@
	
$(H_TEST_BIN): $(H_TEST_OBJ) $(COMMON_OBJ)
	$(CXX) $^ -o $@

# Convenience targets
run: $(RUN_BIN)
	./$(RUN_BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

test_s: $(S_TEST_BIN)
	./$(S_TEST_BIN)
	
test_h: $(H_TEST_BIN)
	./$(H_TEST_BIN)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Include dependency files
-include $(DEPS)

.PHONY: run test test_h clean
