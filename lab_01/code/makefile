CC := gcc
CC_SAN := clang

HEADERS_DIR := inc
SRC_DIR := src
SRC_UNIT_DIR := unit_tests
OUT_DIR := out

SRCS := $(wildcard $(SRC_DIR)/*.c)
UNIT_SRCS := $(wildcard $(SRC_UNIT_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OUT_DIR)/%.o)
UNIT_OBJS := $(UNIT_SRCS:$(SRC_UNIT_DIR)/%.c=$(OUT_DIR)/%.o)
DEPS := $(OBJS:%.o=%.d)
UNIT_DEPS := $(UNIT_OBJS:%.o=%.d)
UNIT_OBJS += $(filter-out $(OUT_DIR)/main.o, $(OBJS))

CFLAGS := -std=c99 -Wall -Werror -Wextra -Wpedantic -Wfloat-equal -Wfloat-conversion -Wvla -O0 -g -I$(HEADERS_DIR)
LINK_FLAGS :=

UNIT_FLAGS := --coverage -O0 -g -fprofile-abs-path
UNIT_LINK_FLAGS := --coverage -lcheck -lm -lrt -lsubunit

JSON_FILE := $(OUT_DIR)/stud-unit-test-report.json

all : app.exe


unit : unit_tests.exe
	printf '{\n"timestamp": "' > $(JSON_FILE)
	date +"%Y-%m-%dT%H:%M:%S%:z" | tr -d '\n' >> $(JSON_FILE)

	printf '",\n"passed": ' >> $(JSON_FILE)
	output=$$(./unit_tests.exe) && echo $$(echo "$$output" | head -n 1) ',' >> $(JSON_FILE) \
	    && echo -n '"failed": ' $$(echo "$$output" | tail -n 1) >> $(JSON_FILE)

	printf ',\n"coverage": ' >> $(JSON_FILE)
	cd $(OUT_DIR) && gcov -s ../$(SRC_DIR) ./*.o | grep -E "^Lines" | tail -n 1 | grep -Eo -m 1 "[0-9]+\.[0-9]+" >> ../$(JSON_FILE)
	echo '}' >> $(JSON_FILE)

format :
	clang-format -style=LLVM -i $(SRC_DIR)/* $(SRC_UNIT_DIR)/* $(HEADERS_DIR)/*

app.exe : $(OBJS)
	$(CC) $^ $(LINK_FLAGS) -o $@

unit_tests.exe : CFLAGS += $(UNIT_FLAGS)
unit_tests.exe : LINK_FLAGS += $(UNIT_LINK_FLAGS)
unit_tests.exe : $(UNIT_OBJS)
	$(CC) $^ $(LINK_FLAGS) -o $@

$(OUT_DIR)/%.o : $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)/check_%.o : $(SRC_UNIT_DIR)/check_%.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_DIR)/%.d : $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) -I$(HEADERS_DIR) -MM -MF $@ -MT $(OUT_DIR)/$*.o $<

$(OUT_DIR)/check_%.d : $(SRC_UNIT_DIR)/check_%.c | $(OUT_DIR)
	$(CC) -I$(HEADERS_DIR) -MM -MF $@ -MT $(OUT_DIR)/$*.o $<

$(OUT_DIR) :
	mkdir -p $@

ifneq (clean, $(MAKECMDGOALS))
  include $(UNIT_DEPS)
  include $(DEPS)
endif

clean :
	$(RM) ./$(OUT_DIR)/* ./*.exe

.PHONY : clean unit all format
