# Pong implemented with SFML Azuxiren's Template

A simple implementation of Pong, using SFML Azuxiren's Template in C++.

## Pre-requisites

- clang or gcc compiler for C++
- [SFML libraries](https://www.sfml-dev.org/download/sfml/2.6.1/) must be installed. In linux, it can be installed easily using

    apt install libsfml-dev
- Optional tools:
    - clang-tidy : For providing analysis
    - clang-format : For formatting the code
    - [bear](https://github.com/rizsotto/Bear): For generation of [compile_commands.json] that will be used by your IDE as well as the above two tools to work properly.


Please go through the Makefile to understand how each tool work for each other. The build command `make build` and `make debug` does not use any of the mentioned optional tools. If the SFML library is loaded at a non-system path rather than being installed system-wide, you will also need to edit the makefile accordingly.


## Build and run

Use the command `make run` to build and run the game. Game is prepared in the `bin` folder.


## Purpose

This template is an initial boilerplate. A game can be abstracted in a series of "Scenes", while the `SfmlGameClass` does the low level tasks for all the scenes, i.e polling for events, storing settings that last for the entire game, invoking the main loop, etc. More details and examples would be provided in the wiki (Hopefully in the near future)

## License

This repository is licensed under the MIT license while the SFML library is licensed under [zlib/libpng](https://github.com/SFML/SFML/blob/master/license.md) license. 
