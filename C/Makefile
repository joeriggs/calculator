
BASE_OBJS = calculator.o      \
            list.o            \
            main.o            \
            operand.o         \
            operand_base_10.o \
            operand_base_16.o \
            operator.o        \
            operator_exp.o    \
            raw_stdin.o       \
            stack.o

TEST ?= 0
ifeq ($(TEST), 1)
	OBJS = test.o $(BASE_OBJS)
	BUILD_FLAGS += -DTEST
	TARGET = test
else
	OBJS = ui.o $(BASE_OBJS)
	TARGET = calculator

endif

DEBUG ?= 0
ifeq ($(DEBUG), 1)
        DEBUG_FLAGS := -O0 -g -DDEBUG
else
        DEBUG_FLAGS := -Os
endif

%.o: %.c
	gcc $(BUILD_FLAGS) $(DEBUG_FLAGS) -Wall -Werror -c -o $@ $<

$(TARGET): $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)

