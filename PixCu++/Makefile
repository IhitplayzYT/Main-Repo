CXX  := g++
NVCC := nvcc

TARGET := PixCu++

SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := build

CPPFLAGS := -I$(INC_DIR)

CXXFLAGS := \
	-O0 \
	-g \
	-std=c++23 \
	-Wall \
	-Wextra \
	-Wpedantic \
	-MMD -MP

NVCCFLAGS := \
	-O0 \
	-g \
	-std=c++23 \
	-arch=sm_120 \
	-I$(INC_DIR)

LDFLAGS :=
LDLIBS :=

CPP_SRC := $(wildcard $(SRC_DIR)/*.cpp)
CU_SRC  := $(wildcard $(SRC_DIR)/*.cu)

CPP_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SRC))
CU_OBJ  := $(patsubst $(SRC_DIR)/%.cu,$(BUILD_DIR)/%.o,$(CU_SRC))

OBJ := $(CPP_OBJ) $(CU_OBJ)

DEP := $(CPP_OBJ:.o=.d)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(NVCC) $(OBJ) $(LDFLAGS) $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cu
	@mkdir -p $(BUILD_DIR)
	$(NVCC) $(NVCCFLAGS) -dc $< -o $@

-include $(DEP)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

run: all
	./$(TARGET)./$(TARGET)
