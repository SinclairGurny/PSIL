all: repl.cpp psil_parser.cpp
	g++ -Wall -std=c++11 repl.cpp psil_parser.cpp -lreadline -o psil

debug: repl.cpp
	g++ -g -Wall -std=c++11 repl.cpp psil_parser.cpp -lreadline -o psil -DDEBUG_MODE

clean:
	$(RM) psil *~
