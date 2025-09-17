CC = gcc
CFLAGS = -std=c99 -g -Wall
SRCDIR = ./CBasicExamples
SRCS = main.c $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

ifneq ($(OS), Windows_NT)
RM = -rm
ifeq ($(shell uname), Linux)
# Enable AddressSanitizer and UndefinedBehaviorSanitizer on Linux
CFLAGS += -fsanitize=address,undefined,leak
endif
else
RM = -del
endif

c_basic_examples: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

-include $(DEPS)

%.o: %.cpp
	$(CC) -MMD -MP -c -o $@ $< $(CFLAGS)

clean:
	$(RM) -f *.o *.d ./CBasicExamples/*.o ./CBasicExamples/*.d c_basic_examples c_basic_examples.exe