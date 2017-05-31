CXX := g++
RM := rm -f
CXXFLAGS := -W -Wall -std=gnu++11
MKDIR_P := mkdir -p
OUT_DIR := ./bin

TEST_TARGET := $(OUT_DIR)/run_tests
TEST_SRC_DIR := ./tests
TEST_OBJ_DIR := $(OUT_DIR)/tests
TEST_INCLUDE := -Isrc/utility -Itests/framework
TEST_FRAMEWORK_DEP := $(wildcard $(TEST_SRC_DIR)/framework/*/*.hpp)
TEST_DEPS := $(TEST_FRAMEWORK_DEP) $(wildcard src/*) $(wildcard src/utility/*)
TEST_SRCS := $(wildcard $(TEST_SRC_DIR)/*.cpp)
TEST_OBJS := $(patsubst $(TEST_SRC_DIR)/%.cpp,$(TEST_OBJ_DIR)/%.o,$(TEST_SRCS))

all: test integration_test

test: $(TEST_TARGET)
	@$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	@echo [Building \"$(TEST_TARGET)\"]
	@$(MKDIR_P) $(OUT_DIR)
	@$(CXX) $(TEST_OBJS) -o $(TEST_TARGET)

$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp $(TEST_DEPS)
	@echo [Compiling \"$<\"]
	@$(MKDIR_P) $(TEST_OBJ_DIR)
	@$(CXX) -c $(CXXFLAGS) $(TEST_INCLUDE) $< -o $@

$(TEST_OBJ_DIR)/Catch.o: $(TEST_SRC_DIR)/Catch.cpp $(TEST_FRAMEWORK_DEP)
	@echo [Compiling \"$<\"]
	@$(MKDIR_P) $(TEST_OBJ_DIR)
	@$(CXX) -c $(CXXFLAGS) $(TEST_INCLUDE) $< -o $@

integration_test:
	@echo "TODO: INTERGRATION TEST (WITH ARDUINO COMPILER)"

clean:
	$(RM) -r $(OUT_DIR)
