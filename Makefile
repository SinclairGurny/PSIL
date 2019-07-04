all: src/repl.cpp src/psil_parser.cpp
	g++ -Wall -std=c++11 src/repl.cpp src/psil_parser.cpp -lreadline -o psil

debug: src/repl.cpp src/psil_parser.cpp
	g++ -g -Wall -std=c++11 src/repl.cpp src/psil_parser.cpp -lreadline -o psil -DDEBUG_MODE

clean:
	$(RM) psil *~ src/*~
