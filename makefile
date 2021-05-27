src = $(wildcard *.c)
target = $(patsubst %.c,%,$(src))

ALL:$(target)

%:%.c
	gcc  $< -o $@ -pthread

clean:
	rm -f $(target)

.PHONY: ALL clean

