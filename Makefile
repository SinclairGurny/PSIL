

all: src/repl.cpp src/psil_parser.cpp src/psil_eval.cpp src/psil_exec.cpp src/psil_exec_funcs.cpp
	mkdir -p build
	g++ -Wall -std=c++17 -c src/psil_parser.cpp -o build/psil_parser.o
	g++ -Wall -std=c++17 -c src/psil_eval.cpp -o build/psil_eval.o
	g++ -Wall -std=c++17 -c src/psil_exec.cpp -o build/psil_exec.o
	g++ -Wall -std=c++17 -g -c src/psil_exec_funcs.cpp -o build/psil_exec_funcs.o
	g++ -Wall -std=c++17 -c src/repl.cpp -o build/repl.o
	g++ build/psil_parser.o build/psil_eval.o build/psil_exec.o build/psil_exec_funcs.o build/repl.o -lreadline -o psil


debug: src/repl.cpp src/psil_parser.cpp src/psil_eval.cpp src/psil_exec.cpp src/psil_exec_funcs.cpp
	mkdir -p build
	g++ -Wall -std=c++17 -g -c src/psil_parser.cpp -DDEBUG_MODE -o build/psil_parser.o
	g++ -Wall -std=c++17 -g -c src/psil_eval.cpp -o build/psil_eval.o
	g++ -Wall -std=c++17 -g -c src/psil_exec.cpp -o build/psil_exec.o
	g++ -Wall -std=c++17 -g -c src/psil_exec_funcs.cpp -o build/psil_exec_funcs.o
	g++ -Wall -std=c++17 -g -c src/repl.cpp -o build/repl.o
	g++ build/psil_parser.o build/psil_eval.o build/psil_exec.o build/psil_exec_funcs.o build/repl.o -lreadline -o psil_debug

debug_slow:  src/repl.cpp src/psil_parser.cpp src/psil_eval.cpp src/psil_exec.cpp src/psil_exec_funcs.cpp
	mkdir -p build
	g++ -Wall -std=c++17 -g -c src/psil_parser.cpp -DDEBUG_MODE -DDEBUG_SLOW -o build/psil_parser.o
	g++ -Wall -std=c++17 -g -c src/psil_eval.cpp -o build/psil_eval.o
	g++ -Wall -std=c++17 -g -c src/psil_exec.cpp -o build/psil_exec.o
	g++ -Wall -std=c++17 -g -c src/psil_exec_funcs.cpp -o build/psil_exec_funcs.o
	g++ -Wall -std=c++17 -g -c src/repl.cpp -o build/repl.o
	g++ build/psil_parser.o build/psil_eval.o build/psil_exec.o build/repl.o -g -lreadline -o psil_debug
	g++ build/psil_parser.o build/psil_eval.o build/psil_exec.o build/psil_exec_funcs.o build/repl.o -g -lreadline -o psil_debug

clean:
	$(RM) psil psil_debug *~ src/*~ docs/*~
	$(RM) -rf build
