CC := gbdk/bin/lcc -Wa-l -Wl-m -Wl-j

.PHONY: all
all: game.gb game.gen.asm game.clang.o

# TODO: Compile with a specific C version (C11?). When I add `std-c89`, per GBDK
# documentation, the compiler hangs.
# https://gbdk-2020.github.io/gbdk-2020/docs/api/docs_coding_guidelines.html
%.gb: %.c
	$(CC) -o $@ $<

# For debugging, it may be useful to review the compiler's assembly output.
%.gen.asm: %.c
	$(CC) -c -S -o $@ $<

# Compile game sources for the host. Clang has better warnings than GBDK.
%.clang.o: %.c
	clang -c -std=c99 \
	  -fno-builtin \
	  -Igbdk/include \
	  -D__PORT_z80 -D__TARGET_gb \
	  -Werror=all -Wno-implicit-int -Wno-unused-value \
	  -o $@ $^

.PHONY: run
run: game.gb
	sdlgnuboy --fullscreen=0 --scale=3 $<

.PHONY: format
format:
	clang-format -i *.c

.PHONY: clean
clean:
	-rm *.gb
	-rm *.map
	-rm *.noi
	-rm *.clang.o
	-rm *.gen.asm
