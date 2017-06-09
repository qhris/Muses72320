CXX := g++
RM := rm -f
CXXFLAGS := -W -Wall -std=gnu++11
MKDIR_P := mkdir -p
OUT_DIR := ./bin
SRC_DIR := src

TEST_TARGET := $(OUT_DIR)/run_tests
TEST_SRC_DIR := ./tests
TEST_OBJ_DIR := $(OUT_DIR)/tests
TEST_INCLUDE := -Itests/mock -I$(SRC_DIR) -Itests/framework
TEST_FRAMEWORK_DEP := $(wildcard $(TEST_SRC_DIR)/framework/*/*.hpp)
TEST_DEPS := $(TEST_FRAMEWORK_DEP) $(wildcard $(SRC_DIR)/*) $(wildcard $(SRC_DIR)/utility/*) $(wildcard $(TEST_SRC_DIR)/mock/*)
TEST_SRCS := $(wildcard $(TEST_SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)
TEST_OBJS := $(patsubst $(TEST_SRC_DIR)/%.cpp,$(TEST_OBJ_DIR)/%.o,$(TEST_SRCS))

all: test integration_test

test: $(TEST_TARGET)
	@$(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	@$(MKDIR_P) $(OUT_DIR)
	$(CXX) $(TEST_OBJS) $(TEST_INCLUDE) -o $(TEST_TARGET)

$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp $(TEST_DEPS)
	@$(MKDIR_P) $(TEST_OBJ_DIR)
	$(CXX) -c $(CXXFLAGS) $(TEST_INCLUDE) $< -o $@

$(TEST_OBJ_DIR)/Catch.o: $(TEST_SRC_DIR)/Catch.cpp $(TEST_FRAMEWORK_DEP)
	@$(MKDIR_P) $(TEST_OBJ_DIR)
	$(CXX) -c $(CXXFLAGS) $(TEST_INCLUDE) $< -o $@

integration_test:
	@echo "TODO: INTERGRATION TEST (WITH ARDUINO COMPILER)"

clean:
	$(RM) -r $(OUT_DIR)
