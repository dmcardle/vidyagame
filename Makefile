CC := gbdk/bin/lcc -Wa-l -Wl-m -Wl-j
ROM := game.gb

.PHONY: all
all: $(ROM)

# TODO: Compile with a specific C version (C11?). When I add `std-c89`, per GBDK
# documentation, the compiler hangs.
# https://gbdk-2020.github.io/gbdk-2020/docs/api/docs_coding_guidelines.html
%.gb: %.c
	$(CC) -o $@ $<

%.clang.o: %.c
	clang -Werror=all -Igbdk/include -o $@ $^

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
