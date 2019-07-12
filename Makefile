

all: src/repl.cpp src/psil_parser.cpp src/psil_eval.cpp src/psil_exec.cpp src/psil_exec_funcs.cpp
	mkdir -p build
	g++ -Wall -std=c++17 -c src/psil_parser.cpp -o build/parser.o
	g++ -Wall -std=c++17 -c src/psil_eval.cpp -o build/eval.o
	g++ -Wall -std=c++17 -c src/psil_exec.cpp -o build/exec.o
	g++ -Wall -std=c++17 -c src/psil_exec_funcs.cpp -o build/funcs.o
	g++ -Wall -std=c++17 -c src/repl.cpp -o build/repl.o
	g++ build/parser.o build/eval.o build/exec.o build/funcs.o build/repl.o -lreadline -o psil


debug: src/repl.cpp src/psil_parser.cpp src/psil_eval.cpp src/psil_exec.cpp src/psil_exec_funcs.cpp
	mkdir -p build
	g++ -Wall -std=c++17 -g -c src/psil_parser.cpp -DDEBUG_MODE -o build/parser.o
	g++ -Wall -std=c++17 -g -c src/psil_eval.cpp -o build/eval.o
	g++ -Wall -std=c++17 -g -c src/psil_exec.cpp -o build/exec.o
	g++ -Wall -std=c++17 -g -c src/psil_exec_funcs.cpp -o build/funcs.o
	g++ -Wall -std=c++17 -g -c src/repl.cpp -o build/repl.o
	g++ build/parser.o build/eval.o build/exec.o build/funcs.o build/repl.o -g -lreadline -o psil_debug

debug_slow:  src/repl.cpp src/psil_parser.cpp src/psil_eval.cpp src/psil_exec.cpp src/psil_exec_funcs.cpp
	mkdir -p build
	g++ -Wall -std=c++17 -g -c src/psil_parser.cpp -DDEBUG_MODE -DDEBUG_SLOW -o build/parser.o
	g++ -Wall -std=c++17 -g -c src/psil_eval.cpp -o build/eval.o
	g++ -Wall -std=c++17 -g -c src/psil_exec.cpp -o build/exec.o
	g++ -Wall -std=c++17 -g -c src/psil_exec_funcs.cpp -o build/funcs.o
	g++ -Wall -std=c++17 -g -c src/repl.cpp -o build/repl.o
	g++ build/parser.o build/eval.o build/exec.o build/funcs.o build/repl.o -g -lreadline -o psil_debug

clean:
	$(RM) psil psil_debug *~ src/*~ docs/*~
	$(RM) -rf build
