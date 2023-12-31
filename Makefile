BEAR=.bear-config.json
COMP=compile_commands.json
TIDY=.clang-tidy
TFLAGS=--checks=modern*,performance*,read*
FORM=.clang-format
BNF=./bin
DEBUG_EXE=./bin/debug.exe
FINAL_EXE=./bin/program.exe
SRC=./src/*.c* ./src/*/*.c*
HPP=./src/*/*.h*
LIB=-lsfml-system -lsfml-graphics -lsfml-window
CC=g++
DBG_BUILD_CMD=$(CC) $(WFLAGS) -g -o $(DEBUG_EXE) $(SRC) $(LIB)

.PHONY:clean format build debug-build run

WFLAGS=-Wall -Wextra -pedantic -std=c++20

$(FINAL_EXE):$(SRC) $(HPP) $(BNF)
	$(CC) $(WFLAGS) -O3 -o $(FINAL_EXE) $(SRC) $(LIB)

$(DEBUG_EXE):$(SRC) $(HPP) $(BNF)
	$(DBG_BUILD_CMD)

$(BNF):
	mkdir $(BNF)

$(FORM):
	clang-format --dump-config > $(FORM)

$(BEAR): 
	mkdir -p $(BNF) && echo '{"compilation": null,"output": {"content":{"include_only_existing_source": true,"duplicate_filter_fields": "file"},"format": {"command_as_array": true,"drop_output_field": false}}}' > $(BEAR)

$(COMP): $(BEAR)
	bear --config $(BEAR) -- $(DBG_BUILD_CMD)

$(TIDY):
	clang-tidy --checks='clang-*,modernize-*,read*,performance-*' --dump-config > $(TIDY)

clean:
	rm -f ./bin/*.exe

format: $(FORM) $(SRC) $(HPP)
	clang-format -i $(SRC) $(HPP)

tidy: $(COMP) $(TIDY) $(SRC)
	clang-tidy --config-file=$(TIDY) $(SRC) $(HPP)

flush-tidy:
	rm -f .clang-tidy compile_commands.json

debug:$(DEBUG_EXE)

build:$(FINAL_EXE)

run:$(FINAL_EXE)
	$(FINAL_EXE)