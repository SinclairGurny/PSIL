/**
   psil_exec_funcs.cpp
   PSIL Execution Library Global function implementations
   @author Sinclair Gurny
   @version 0.5
   July 2019
*/

#include "psil_exec.h"

namespace psil_exec {


  // Take function name and apply correct function
  void apply_global_proc( stack_ptr & s, token_ptr & node, bool& rem, std::string fun ) {
    size_t arg_count = node->aspects.front()->tk->aspects.size()-3;
    // Input / Output ====================================================================
    if ( fun == "print" ) {
      if ( arg_count < 1 )
	throw std::string( "print: Wrong number of arguments given, 1+ expected" );
      rem = true;
      print( node->aspects.front()->tk, false);
    } else if ( fun == "println" ) {
      if ( arg_count < 1 )
	throw std::string( "println: Wrong number of arguments given, 1+ expected" );
      rem = true;
      print( node->aspects.front()->tk, true );
    } else if ( fun == "read" ) {
      if ( arg_count != 0 )
	throw std::string( "read: Wrong number of arguments given, 0 expected" );
      psil_read( node );
    } else if ( fun == "newline" ) {
      if ( arg_count != 0 )
	throw std::string( "newline: Wrong number of arguments given, 0 expected" );
      rem = true;
      std::cout << std::endl;
    }
    // Boolean operations  ================================================================
    else if ( fun == "and" ) {
      if ( arg_count < 2 )
	throw std::string( "and: Wrong number of arguments given, 2+ expected" );
      psil_and( s, node );
    } else if ( fun == "or" ) {
      if ( arg_count < 2 )
	throw std::string( "or: Wrong number of arguments given, 2+ expected" );
      psil_or( s, node );
    } else if ( fun == "not" ) {
      if ( arg_count != 1 )
	throw std::string( "not: Wrong number of arguments given, 1 expected" );
      psil_not( s, node );
    } else if ( fun == "equal?" ) {
      if ( arg_count != 2 )
	throw std::string( "equal?: Wrong number of arguments given, 2 expected" );
      psil_is_equal( s, node );
    }
    // Arithmetic  ====================================================================
    else if ( fun == "+" ) {
      if ( arg_count < 1 )
	throw std::string( "+: Wrong number of arguments given, 1+ expected" );
      psil_add( s, node );
    } else if ( fun == "-" ) {
      if ( arg_count < 1 )
	throw std::string( "-: Wrong number of arguments given, 1+ expected" );
      psil_sub( s, node );
    } else if ( fun == "*" ) {
      if ( arg_count < 1 )
	throw std::string( "*: Wrong number of arguments given, 1+ expected" );
      psil_mult( s, node );
    } else if ( fun == "/" ) {
      if ( arg_count < 1 )
	throw std::string( "/: Wrong number of arguments given, 1+ expected" );
      psil_div( s, node );
    }
    // Approx  ===========================================================================
    else if ( fun == "abs" ) {
      if ( arg_count != 1 )
	throw std::string( "abs: Wrong number of arguments given, 1 expected" );

    } else if ( fun == "mod" ) {
      if ( arg_count != 2 )
	throw std::string( "mod: Wrong number of arguments given, 2 expected" );

    } else if ( fun == "floor" ) {
      if ( arg_count != 1 )
	throw std::string( "floor: Wrong number of arguments given, 1 expected" );

    } else if ( fun == "ceil" ) {
      if ( arg_count != 1 )
	throw std::string( "ceil: Wrong number of arguments given, 1 expected" );

    } else if ( fun == "trunc" ) {
      if ( arg_count != 1 )
	throw std::string( "trunc: Wrong number of arguments given, 1 expected" );

    } else if ( fun == "round" ) {
      if ( arg_count != 1 )
	throw std::string( "round: Wrong number of arguments given, 1 expected" );

    }
    // Inequalities  ====================================================================
    else if ( fun == "lt" ) {
      if ( arg_count != 2 )
	throw std::string( "lt: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "lte" ) {
      if ( arg_count != 2 )
	throw std::string( "lte: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "gt" ) {
      if ( arg_count != 2 )
	throw std::string( "gt: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "gte" ) {
      if ( arg_count != 2 )
	throw std::string( "gte: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "eq" ) {
      if ( arg_count != 2 )
	throw std::string( "eq: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "zero?" ) {
      if ( arg_count != 2 )
	throw std::string( "zero?: Wrong number of arguments given, 1+ expected" );

    }
    // Character  ====================================================================
    else if ( fun == "chlt" ) {
      if ( arg_count != 2 )
	throw std::string( "chlt: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "chlte" ) {
      if ( arg_count != 2 )
	throw std::string( "chlte: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "chgt" ) {
      if ( arg_count != 2 )
	throw std::string( "chgt: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "chgte" ) {
      if ( arg_count != 2 )
	throw std::string( "chgte: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "cheq" ) {
      if ( arg_count != 2 )
	throw std::string( "cheq: Wrong number of arguments given, 1+ expected" );

    }
    // List  ==========================================================================
    else if ( fun == "first" ) {
      if ( arg_count != 2 )
	throw std::string( "first: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "second" ) {
      if ( arg_count != 2 )
	throw std::string( "second: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "nth" ) {
      if ( arg_count != 2 )
	throw std::string( "nth: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "first!" ) {
      if ( arg_count != 2 )
	throw std::string( "first!: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "second!" ) {
      if ( arg_count != 2 )
	throw std::string( "second: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "nth!" ) {
      if ( arg_count != 2 )
	throw std::string( "nth: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "null?" ) {
      if ( arg_count != 1 )
	throw std::string( "null?: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "quote" ) {
      if ( arg_count != 1 )
	throw std::string( "quote: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "unquote" ) {
      if ( arg_count != 1 )
	throw std::string( "unquote: Wrong number of arguments given, 1+ expected" );

    }
    // Identity  ====================================================================
    else if ( fun == "bool?" ) {
      if ( arg_count != 1 )
	throw std::string( "bool?: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "number?" ) {
      if ( arg_count != 1 )
	throw std::string( "number?: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "char?" ) {
      if ( arg_count != 1 )
	throw std::string( "char: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "symbol?" ) {
      if ( arg_count != 1 )
	throw std::string( "println: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "proc?" ) {
      if ( arg_count != 1 )
	throw std::string( "println: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "list?" ) {
      if ( arg_count != 1 )
	throw std::string( "println: Wrong number of arguments given, 1+ expected" );

    }
  }

  // ================= INPUT / OUTPUT ==========================================================

  // converts psil character to char
  char psil_char( std::string ch ) {
    if ( ch.size() > 3 ) {
      if ( ch == "#\\newline" ) {
	return '\n';
      } else if ( ch == "#\\space" ) {
	return ' ';
      } else if ( ch == "#\\tab" ) {
	return '\t';
      } else if ( ch == "#\\oparen" ) {
	return '(';
      } else if ( ch == "#\\cparen" ) {
	return ')';
      } else if ( ch == "#\\osqbrac" ) {
	return '[';
      } else if ( ch == "#\\csqbrac" ) {
	return ']';
      }
    }
    return ch[2];
  }

  // converts char to psil character
  std::string psil_char( char c ) {
    std::string val;
    switch ( c ) {
    case ' ':
      val = "#\\space";
      break;
    case '\t':
      val = "#\\tab";
      break;
    case '(':
      val = "#\\oparen";
      break;
    case ')':
      val = "#\\cparen";
      break;
    case '[':
      val = "#\\osqbrac";
      break;
    case ']':
      val = "#\\csqbrac";
      break;
    case '\n':
      val = "#\\newline";
      break;
    default:
      val = "#\\";
      val += c;
    }
    return val;
  }

  std::string tk_to_string( token_ptr & tk ) {
    // if constant
    //   print
    // if procedure then PROCEDURE!
    return "";
  }

  // Prints constant types to string
  void print( token_ptr & node, bool newline ) {
    size_t idx = 0;
    for ( auto itr = node->aspects.begin(); itr != node->aspects.end(); ++itr, ++idx ) {
      if ( idx > 1 && idx < node->aspects.size() - 1 ) { // Just arguments of function call
	if ( (*itr)->elem_type == TE_Type::TOKEN && (*itr)->tk->aspects.size() == 1 &&
	     (*itr)->tk->aspects.front()->elem_type == TE_Type::TOKEN ) {
	  if ( (*itr)->tk->aspects.front()->tk->type_name == "<constant>" ) {
	    auto const_type = (*itr)->tk->aspects.front()->tk->aspects.front()->tk.get();
	    if ( const_type->type_name == "<boolean>" ) {
	      std::cout << const_type->aspects.front()->str;
	    } else if ( const_type->type_name == "<number>" ) {
	      auto num_type = const_type->aspects.front()->tk.get();
	      auto val = num_type->aspects.front()->str;
	      std::cout << val;
	    } else if ( const_type->type_name == "<character>" ) {
	      std::string ch = const_type->aspects.front()->str;
	      
	    } else if ( const_type->type_name == "<list_def>" ) {
	      std::cerr << "Cannot output lists yet" << std::endl;
	      // TODO
	    }
	  }
	}
      }
    }
    if ( newline )
      std::cout << std::endl;
  }

  // Reads from cin, converts string to list or characters
  void psil_read( token_ptr & node ) {
    std::cout << "READ" << std::endl;
    std::string str;
    std::cin >> str;

    // Reset application to <constant>
    node->aspects.front()->tk->type_name = "<constant>";
    node->aspects.front()->tk->aspects.clear();
    // <constant> -> <list_def>
    auto tmp_top = std::make_unique<psil_parser::token_t>("<list_def>");
    // <list_def> -> (quote <datum>)
    auto tmp_mid = std::make_unique<psil_parser::token_t>("<datum>");
    // <datum> -> <list> -> (...)
    auto tmp_bot = std::make_unique<psil_parser::token_t>("<list>");
    // Add starting paren to list
    tmp_bot->aspects.push_back( std::make_unique<psil_parser::token_elem_t>("(") );
    
    for ( char c : str ) {
      std::string val;

      // Create character datum
      auto tmp_char_top = std::make_unique<psil_parser::token_t>("<datum>");
      auto tmp_char_bot = std::make_unique<psil_parser::token_t>("<character>");
      // Add value to character
      tmp_char_bot->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( val ) );
      // Add character to datum
      auto ch_elem_bot = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_char_bot ) );
      tmp_char_top->aspects.push_back( std::move( ch_elem_bot ) );
      // Add character to list's datum
      auto ch_elem_top = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_char_top ) );
      tmp_bot->aspects.push_back( std::move( ch_elem_top ) );
    }

    // Add ending paren to list
    tmp_bot->aspects.push_back( std::make_unique<psil_parser::token_elem_t>(")") );
    // Add list to <datum>  
    auto elem_bot = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_bot ) );
    tmp_mid->aspects.push_back( std::move( elem_bot ) );
    
    // Build <list_def>
    tmp_top->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( "(" ) );
    tmp_top->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( "quote" ) );
    auto elem_mid = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_mid ) );
    tmp_top->aspects.push_back( std::move( elem_mid ) );
    tmp_top->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( ")" ) );
    // Connect <list_def> to <constant>
    auto elem_top = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_top ) );
    node->aspects.front()->tk->aspects.push_back( std::move( elem_top ) );

  }

}
