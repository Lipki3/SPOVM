TARGET := lab

C     := clang
FLAGS := -W -Wall -Wextra

SRC := $(wildcard *.c)
OBJ := $(SRC:%.c=%.o)
DEP := $(SRC:%.c=%.d)

$(TARGET): $(OBJ)
	$(C) -o $@ $^

%.o: %.c
	$(C) $(FLAGS) -MMD -MP -MF $*.d -o $@ -c $<

$(DEP):

install:
	cp -f $(TARGET) /usr/local/bin/$(TARGET)

uninstall:
	rm -f /usr/local/bin/$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(OBJ)
	rm -f $(DEP)

-include $(DEP)
