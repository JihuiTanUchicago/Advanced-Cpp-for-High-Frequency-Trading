# ---------- build configuration ----------
CXX      := g++
CXXFLAGS := -std=c++20 -O2 -Wall -Wextra -pedantic -Iinclude
LDFLAGS  :=                                # add libraries here if needed

# ---------- file layout ----------
SRC_DIR  := src
OBJ_DIR  := build
BIN_DIR  := bin
TARGET   := $(BIN_DIR)/orderbook_demo           # final executable name

# ---------- source / object lists ----------
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# ---------- default rule ----------
all: $(TARGET)

# ---------- link step ----------
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

# ---------- compile step ----------
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ---------- directory guards ----------
$(BIN_DIR) $(OBJ_DIR):
	@mkdir -p $@

# ---------- convenience ----------
run: $(TARGET)
	@./$(TARGET)

clean:
	@$(RM) -r $(OBJ_DIR) $(BIN_DIR)

rebuild: clean all

.PHONY: all run clean rebuild
