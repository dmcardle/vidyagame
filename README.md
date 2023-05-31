# Vidyagame

[![Build](https://github.com/dmcardle/vidyagame/actions/workflows/build.yml/badge.svg)](https://github.com/dmcardle/vidyagame/actions/workflows/build.yml)

This is my toy video game, written in C, that runs on Nintendo's original Game
Boy!

## Compiling the ROM

Before you can compile the ROM, you'll need to run the setup script. It fetches
GBDK, installs packages, etc. The supported compilation environment is whatever
happens to be on my computer --- currently, that's Ubuntu 22.10.

```sh
./setup.sh
```

Now that the environment is set up, you can use `make` to build the ROM.

``` sh
# Compile the ROM.
make game.gb

# Run the ROM in an emulator.
make run
```
