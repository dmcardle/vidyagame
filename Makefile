CC := gbdk/bin/lcc -Wa-l -Wl-m -Wl-j
ROM := game.gb

.PHONY: all
all: $(ROM)

%.gb: %.c
	$(CC) -o $@ $<

.PHONY: run
run: $(ROM)
	gngb -o $(ROM)

.PHONY: format
format:
	clang-format -i *.c

.PHONY: clean
clean:
	-rm *.gb
	-rm *.map
	-rm *.noi
