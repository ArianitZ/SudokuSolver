GCC = g++

COMPILER_FLAGS = -g

OUTPUT = bin/sudoku_solver

DEPS = src/*.cpp

sudoku_solver: ${DEPS}
	${GCC} ${COMPILER_FLAGS} ${DEPS} -o ${OUTPUT}