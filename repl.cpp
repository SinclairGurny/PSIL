/* PSIL REPL code
 * Written by Sinclair Gurny
 * July 2019
 */

// Cpp includes
#include <iostream>
#include <string>
// C includes
#include <cstdlib>
#include <csignal>
// Other libaries
#include <readline/readline.h>
#include <readline/history.h>
// PSIL Parser
#include "psil_parser.h"

// Handles Signals
void exitHandler( int signum ) {
  std::cout << "\nInterrupt signal (" << signum << ") received." << std::endl;
  std::cout << "Exiting PSIL REPL" << std::endl;
  clear_history();
  exit( 0 );  
}

int main( int argc, char ** argv ) {
  
  std::cout << "\n***********************************************\n";
  std::cout << "***_______*****_________****_______****___******\n";
  std::cout << "**|\\   _  \\***|\\   _____\\**/\\__   _\\**|\\  \\******\n";
  std::cout << "**\\ \\  \\\\  \\**\\ \\  \\____|**\\/_ \\  \\/**\\ \\  \\******\n";
  std::cout << "***\\ \\  \\\\  \\**\\ \\  \\_____****\\ \\  \\***\\ \\  \\******\n";
  std::cout << " ***\\ \\  ___/***\\ \\____   \\****\\ \\  \\***\\ \\  \\******\n";
  std::cout << "  ***\\ \\ \\_/*****\\/____|\\  \\****\\ \\  \\***\\ \\  \\******\n";
  std::cout << "   ***\\ \\ \\*********____\\\\  \\****\\_\\  \\_**\\ \\  \\_____*\n";
  std::cout << "    ***\\ \\_\\*******/\\________\\**/\\      \\**\\ \\_______\\*\n";
  std::cout << "     ***\\/_/*******\\/________/**\\/______/***\\/_______/*\n";
  std::cout << "      *************************************************\n\n";
  std::cout << "\"It's like LISP but not!\"\n\n";
  std::cout << "==== PSIL REPL v0.1 =====\n";
  std::cout << "Press Ctrl+C to Exit\n" << std::endl;

  // === Setup ===
  signal( SIGINT, exitHandler);
  // Make PSIL Language
  psil::language_ptr psil_lang = psil::make_psil_lang();
  
  // === REPL ===
  while ( true ) {
    char * buffer = readline("psil>> ");

    add_history( buffer );

    std::string tmp_buf(buffer);
    
    if ( tmp_buf == "exit" || tmp_buf == "quit") {
      // === Delete memory and exit ===
      free(buffer);
      delete_lang( psil_lang );
      exit(0);
    } else if ( tmp_buf == "psil" ) {
      // === Display PSIL Syntax ===
      lang_print( psil_lang );
      free(buffer);
      continue;
    } else if ( tmp_buf == "help" ) {
      std::cout << "psil - prints PSIL language syntax\n"
		<< "exit/quit - exits PSIL REPL" << std::endl;
      free(buffer);
      continue;
    }

    psil::token_ptr out = psil::parse( psil_lang, tmp_buf );
    tk_print( out );
    out.reset();
    
    std::cout << "You entered: " << buffer << std::endl;

    free(buffer);
  }

  return 0;
}
