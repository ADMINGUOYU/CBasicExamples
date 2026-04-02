CC = gcc
CFLAGS = -std=c99 -g -Wall
SRCDIR = ./CBasicExamples
# Collect all .c files in the source directory and its subdirectories
SRCS = main.c $(wildcard $(SRCDIR)/*.c)
# Add source files from subdirectories
SRCS += $(wildcard $(SRCDIR)/*/*.c)
SRCS += $(wildcard $(SRCDIR)/*/*/*.c)
# Generate object files and dependency files from source files
OBJS = $(SRCS:.c=.o)
# Generate dependency files from source files
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

%.o: %.c
	$(CC) -MMD -MP -c -o $@ $< $(CFLAGS)

clean:
	$(RM) $(OBJS) $(DEPS) c_basic_examples