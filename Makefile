

OBJ = build/parser.o build/eval.o build/exec.o build/funcs.o build/bool.o build/comp.o \
	build/list.o build/math.o build/types.o build/repl.o

PARSE_H = src/psil_parser.h
PARSE_CPP = src/psil_parser.cpp

EVAL_H = src/psil_eval.h
EVAL_CPP = src/psil_eval.cpp

EXEC_H = src/psil_exec.h
EXEC_CPP = src/psil_exec.cpp src/psil_exec_funcs.cpp src/psil_exec_bool.cpp src/psil_exec_comp.cpp \
		src/psil_exec_list.cpp src/psil_exec_math.cpp src/psil_exec_types.cpp

MAIN_H = src/psil.h
MAIN_CPP = src/repl.cpp

ALL_H = $(PARSE_H) $(EVAL_H) $(EXEC_H) $(MAIN_H)
ALL_CPP =  $(PARSE_CPP) $(EVAL_CPP) $(EXEC_CPP) $(MAIN_CPP)



ifdef DEBUG
	EXTRAS = -g -DDEBUG_MODE
	OUTPUT = psil_debug
else
	EXTRAS = -O3
	OUTPUT = psil
endif

FLAGS = -Wall -std=c++17 $(EXTRAS)

LIBS = -lreadline

all: $(OUTPUT)
	@echo '... Finished Compiling PSIL ...'

psil: $(OBJ)
	g++ $(OBJ) $(LIBS) -o $(OUTPUT)

psil_debug: $(OBJ)
	g++ $(OBJ) $(LIBS) -o $(OUTPUT)

debug:
	rm -f build/*o
	$(MAKE) DEBUG=1

build/parser.o: $(PARSE_H) $(PARSE_CPP)
	mkdir -p build
	g++ $(FLAGS) -c $(PARSE_CPP) -o build/parser.o

build/eval.o: $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) -c $(EVAL_CPP) -o build/eval.o

build/exec.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) -c src/psil_exec.cpp -o build/exec.o

build/funcs.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) -c src/psil_exec_funcs.cpp -o build/funcs.o

build/bool.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) -c src/psil_exec_bool.cpp -o build/bool.o

build/comp.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) -c src/psil_exec_comp.cpp -o build/comp.o

build/list.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) -c src/psil_exec_list.cpp -o build/list.o

build/math.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) -c src/psil_exec_math.cpp -o build/math.o

build/types.o: $(EXEC_H) $(EXEC_CPP) $(EVAL_H) $(EVAL_CPP) $(PARSE_H) $(PARSE_CPP)
	g++ $(FLAGS) -c src/psil_exec_types.cpp -o build/types.o

build/repl.o: $(ALL_H) $(ALL_CPP)
	g++ $(FLAGS) -c src/repl.cpp $(LIBS) -o build/repl.o

clean:
	$(RM) psil psil_debug *~ src/*~ docs/*~ examples/*~
	$(RM) -rf build
