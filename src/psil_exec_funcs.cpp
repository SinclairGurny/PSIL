/**
   psil_exec_funcs.cpp
   PSIL Execution Library Global function implementations
   @author Sinclair Gurny
   @version 0.1
   July 2019
*/

#include "psil_exec.h"

namespace psil_exec {

  void print( token_ptr & node, bool newline ) {
    size_t idx = 0;
    for ( auto itr = node->aspects.begin(); itr != node->aspects.end(); ++itr, ++idx ) {
      if ( idx > 1 && idx < node->aspects.size() - 1 ) {
	if ( (*itr)->elem_type == TE_Type::TOKEN && (*itr)->tk->aspects.size() == 1 &&
	     (*itr)->tk->aspects.front()->elem_type == TE_Type::TOKEN ) {
	  if ( (*itr)->tk->aspects.front()->tk->type_name == "<constant>" ) {
	    auto const_type = (*itr)->tk->aspects.front()->tk->aspects.front()->tk.get();
	    if ( const_type->type_name == "<boolean>" ) {
	      std::cout << const_type->aspects.front()->str;
	    } else if ( const_type->type_name == "<number>" ) {
	      // TODO
	    } else if ( const_type->type_name == "<character>" ) {
	      std::string ch = const_type->aspects.front()->str;
	      if ( ch.size() > 3 ) {
		if ( ch == "#\\newline" ) {
		  std::cout << "\n";
		} else if ( ch == "#\\space" ) {
		  std::cout << " ";
		} else if ( ch == "#\\oparen" ) {
		  std::cout << "(";
		} else if ( ch == "#\\cparen" ) {
		  std::cout << ")";
		} else if ( ch == "#\\osqbrac" ) {
		  std::cout << "[";
		} else if ( ch == "#\\csqbrac" ) {
		  std::cout << "]";
		}
	      } else {
		std::cout << ch.substr(2);
	      }
	    }
	  }
	}
      }
    }
    if ( newline )
      std::cout << std::endl;
  }
  

}
