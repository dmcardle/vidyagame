CC := gbdk/bin/lcc -Wa-l -Wl-m -Wl-j
ROM := game.gb

.PHONY: all
all: $(ROM)

%.gb: %.c
	$(CC) -o $@ $<

.PHONY: run
run: $(ROM)
	sdlgnuboy --fullscreen=0 --scale=3 $<

.PHONY: format
format:
	clang-format -i *.c

.PHONY: clean
clean:
	-rm *.gb
	-rm *.map
	-rm *.noi
