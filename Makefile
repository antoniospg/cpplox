# Name of the project
PROJ_NAME=cpplox
 
# .c files
CPP_src=$(wildcard ./src/*.cpp)
 
# .h files
H_src=$(wildcard ./src/*.h)
 
# Object files
OBJ=$(subst .cpp,.o,$(subst src,objects,$(CPP_src)))
 
# Compiler and linker
CC=clang++
 
# Flags for compiler
CC_FLAGS=-c         \
         -W         \
         -Wall      \
         -ansi      \
         -pedantic \
				 -std=c++17
 
# Command used at clean target
RM = rm -rf
 
#
# Compilation and linking
#
all: objFolder $(PROJ_NAME)
 
$(PROJ_NAME): $(OBJ)
	@ echo 'Building binary using GCC linker: $@'
	$(CC) $^ -o $@
	@ echo 'Finished building binary: $@'
	@ echo ' '
 
./objects/%.o: ./src/%.cpp ./src/%.h 
	@ echo 'Building target using GCC compiler: $<' 
	$(CC) $< $(CC_FLAGS) -o $@ 
	@ echo ' '
 
./objects/main.o: ./src/main.cpp $(H_src)
	@ echo 'Building target using GCC compiler: $<'
	$(CC) $< $(CC_FLAGS) -o $@
	@ echo ' '
 
objFolder:
	@ mkdir -p objects
 
clean:
	@ $(RM) ./objects/*.o $(PROJ_NAME) *~
	@ rmdir objects
 
.PHONY: all clean
