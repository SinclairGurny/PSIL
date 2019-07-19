
# All .o files
OBJ = build/parser.o build/eval.o build/exec.o build/funcs.o build/bool.o build/comp.o \
	build/list.o build/math.o build/types.o build/repl.o

DEBUG_OBJ = build/dparser.o build/deval.o build/dexec.o build/dfuncs.o build/dbool.o build/dcomp.o \
	build/dlist.o build/dmath.o build/dtypes.o build/drepl.o

# Parsing Library
PARSE_H = src/psil_parser.h
PARSE_CPP = src/psil_parser.cpp
# Evaluation Library
EVAL_H = src/psil_eval.h
EVAL_CPP = src/psil_eval.cpp
# Execution Library
EXEC_H = src/psil_exec.h
EXEC_CPP = src/psil_exec.cpp src/psil_exec_funcs.cpp src/psil_exec_bool.cpp src/psil_exec_comp.cpp \
		src/psil_exec_list.cpp src/psil_exec_math.cpp src/psil_exec_types.cpp
# Main Code
MAIN_H = src/psil.h
MAIN_CPP = src/repl.cpp

# All header and cpp files
ALL_H = $(PARSE_H) $(EVAL_H) $(EXEC_H) $(MAIN_H)
ALL_CPP =  $(PARSE_CPP) $(EVAL_CPP) $(EXEC_CPP) $(MAIN_CPP)

FLAGS = -Wall -std=c++17
OPT_FLAGS = -O3
DEBUG_FLAGS = -g

LIBS = -lreadline

normal: $(OBJ)
	g++ $(OBJ) $(LIBS) -o psil
	@echo "... Finished Building PSIL ..."

debug: $(DEBUG_OBJ)
	g++ $(DEBUG_OBJ) $(LIBS) -o psil_debug
	@echo "... Finished Building DEBUG PSIL ..."

# Normal Building rules
build/parser.o: $(PARSE_H) $(PARSE_CPP)
	mkdir -p build
	g++ $(FLAGS) $(OPT_FLAGS) -c $(PARSE_CPP) -o build/parser.o

build/eval.o: $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) $(OPT_FLAGS) -c $(EVAL_CPP) -o build/eval.o

build/exec.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) $(OPT_FLAGS) -c src/psil_exec.cpp -o build/exec.o

build/funcs.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) $(OPT_FLAGS) -c src/psil_exec_funcs.cpp -o build/funcs.o

build/bool.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) $(OPT_FLAGS) -c src/psil_exec_bool.cpp -o build/bool.o

build/comp.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) $(OPT_FLAGS) -c src/psil_exec_comp.cpp -o build/comp.o

build/list.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) $(OPT_FLAGS) -c src/psil_exec_list.cpp -o build/list.o

build/math.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) $(OPT_FLAGS) -c src/psil_exec_math.cpp -o build/math.o

build/types.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) $(OPT_FLAGS) -c src/psil_exec_types.cpp -o build/types.o

build/repl.o: $(ALL_H) $(ALL_CPP)
	g++ $(FLAGS) $(OPT_FLAGS) -c src/repl.cpp $(LIBS) -o build/repl.o


# DEBUG Building rules
build/dparser.o: $(PARSE_H) $(PARSE_CPP)
	mkdir -p build
	g++ $(FLAGS) $(DEBUG_FLAGS) -c $(PARSE_CPP) -o build/dparser.o

build/deval.o: $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) $(DEBUG_FLAGS) -c $(EVAL_CPP) -o build/deval.o

build/dexec.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) $(DEBUG_FLAGS) -c src/psil_exec.cpp -o build/dexec.o

build/dfuncs.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) $(DEBUG_FLAGS) -c src/psil_exec_funcs.cpp -o build/dfuncs.o

build/dbool.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) $(DEBUG_FLAGS) -c src/psil_exec_bool.cpp -o build/dbool.o

build/dcomp.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) $(DEBUG_FLAGS) -c src/psil_exec_comp.cpp -o build/dcomp.o

build/dlist.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) $(DEBUG_FLAGS) -c src/psil_exec_list.cpp -o build/dlist.o

build/dmath.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) $(DEBUG_FLAGS) -c src/psil_exec_math.cpp -o build/dmath.o

build/dtypes.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) $(DEBUG_FLAGS) -c src/psil_exec_types.cpp -o build/dtypes.o

build/drepl.o: $(ALL_H) $(ALL_CPP)
	g++ $(FLAGS) $(DEBUG_FLAGS) -c src/repl.cpp $(LIBS) -o build/drepl.o


clean:
	$(RM) psil psil_debug *~ src/*~ docs/*~ examples/*~
	$(RM) -rf build
