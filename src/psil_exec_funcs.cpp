/**
   psil_exec_funcs.cpp
   PSIL Execution Library Global function implementations
   @author Sinclair Gurny
   @version 0.1
   July 2019
*/

#include "psil_exec.h"

namespace psil_exec {

  void apply_global_proc( stack_ptr & s, token_ptr & node, bool& rem, std::string fun ) {
    // Input / Output
    if ( fun == "print" ) {
      if ( node->aspects.size() < 4 )
	throw std::string( "print: Wrong number of arguments given, 1+ expected" );
      rem = true;
      print( node, false);
    } else if ( fun == "println" ) {
      if ( node->aspects.size() < 4 )
	throw std::string( "println: Wrong number of arguments given, 1+ expected" );
      rem = true;
      print( node , true );
    } else if ( fun == "newline" ) {
      if ( node->aspects.size() != 3 )
	throw std::string( "newline: Wrong number of arguments given, 0 expected" );
      rem = true;
      std::cout << std::endl;
    }
    // Boolean operations
    else if ( fun == "and" ) {
      if ( node->aspects.size() < 5 )
	throw std::string( "and: Wrong number of arguments given, 2+ expected" );
      psil_and( s, node );
    } else if ( fun == "or" ) {
      if ( node->aspects.size() < 5 )
	throw std::string( "or: Wrong number of arguments given, 2+ expected" );
      psil_or( s, node );
    } else if ( fun == "not" ) {
      if ( node->aspects.size() != 4 )
	throw std::string( "not: Wrong number of arguments given, 1 expected" );
      psil_not( s, node );
    } else if ( fun == "equal?" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "equal?: Wrong number of arguments given, 2 expected" );

    }
    // Arithmetic
    else if ( fun == "+" ) {
      if ( node->aspects.size() < 4 )
	throw std::string( "+: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "-" ) {
      if ( node->aspects.size() < 4 )
	throw std::string( "-: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "*" ) {
      if ( node->aspects.size() < 4 )
	throw std::string( "*: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "/" ) {
      if ( node->aspects.size() < 4 )
	throw std::string( "/: Wrong number of arguments given, 1+ expected" );

    }
    // Approx
    else if ( fun == "abs" ) {
      if ( node->aspects.size() != 4 )
	throw std::string( "abs: Wrong number of arguments given, 1 expected" );

    } else if ( fun == "mod" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "mod: Wrong number of arguments given, 2 expected" );

    } else if ( fun == "floor" ) {
      if ( node->aspects.size() != 4 )
	throw std::string( "floor: Wrong number of arguments given, 1 expected" );

    } else if ( fun == "ceil" ) {
      if ( node->aspects.size() != 4 )
	throw std::string( "ceil: Wrong number of arguments given, 1 expected" );

    } else if ( fun == "trunc" ) {
      if ( node->aspects.size() != 4 )
	throw std::string( "trunc: Wrong number of arguments given, 1 expected" );

    } else if ( fun == "round" ) {
      if ( node->aspects.size() != 4 )
	throw std::string( "round: Wrong number of arguments given, 1 expected" );

    }
    // Inequalities
    else if ( fun == "lt" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "lt: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "lte" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "lte: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "gt" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "gt: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "gte" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "gte: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "eq" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "eq: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "zero?" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "zero?: Wrong number of arguments given, 1+ expected" );

    }
    // Character
    else if ( fun == "chlt" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "chlt: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "chlte" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "chlte: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "chgt" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "chgt: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "chgte" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "chgte: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "cheq" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "cheq: Wrong number of arguments given, 1+ expected" );

    }
    // List
    else if ( fun == "first" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "first: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "second" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "second: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "nth" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "nth: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "first!" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "first!: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "second!" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "second: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "nth!" ) {
      if ( node->aspects.size() != 5 )
	throw std::string( "nth: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "null?" ) {
      if ( node->aspects.size() != 4 )
	throw std::string( "null?: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "quote" ) {
      if ( node->aspects.size() != 4 )
	throw std::string( "quote: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "unquote" ) {
      if ( node->aspects.size() != 4 )
	throw std::string( "unquote: Wrong number of arguments given, 1+ expected" );

    }
    // Identity
    else if ( fun == "bool?" ) {
      if ( node->aspects.size() != 4 )
	throw std::string( "bool?: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "number?" ) {
      if ( node->aspects.size() != 4 )
	throw std::string( "number?: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "char?" ) {
      if ( node->aspects.size() != 4 )
	throw std::string( "char: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "symbol?" ) {
      if ( node->aspects.size() != 4 )
	throw std::string( "println: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "proc?" ) {
      if ( node->aspects.size() != 4 )
	throw std::string( "println: Wrong number of arguments given, 1+ expected" );

    } else if ( fun == "list?" ) {
      if ( node->aspects.size() != 4 )
	throw std::string( "println: Wrong number of arguments given, 1+ expected" );

    }
  }

  // ================= INPUT / OUTPUT ==========================================================

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


  
  void psil_read( token_ptr & node ) {
    std::string str;
    std::cin >> str;
    // make expression
    // make quote parts
    //for ( char c : str ) {
    // make char token, add to quote token
    // append char into quote
    //}
    // add final
  }

  // ========================= BOOLEAN OPERATIONS ================================================
  
  // Performs logical and on all arguments
  void psil_and( stack_ptr & s, token_ptr & node ) {
    std::cout << "AND" << std::endl;
    size_t idx = 0;
    std::string val = "";
    //bool result = true;
    for ( auto itr = node->aspects.begin(); itr != node->aspects.end(); ++itr, ++idx ) {
      if ( idx > 1 && idx < node->aspects.size() - 1 ) { // Just arguments of function
	if ( !is_true( s, (*itr)->tk ) ) {
	  val = "#f";
	  break;
	}
      }
    }
    if ( val.empty() ) { val = "#t"; }
    std::cout << "AND: " << val << std::endl;
    node->type_name = "<constant>";
    node->aspects.clear();
    // create boolean token
    auto tmp_bool = std::make_unique<psil_parser::token_t>("<boolean>");
    // add value
    tmp_bool->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( val ) );
    // add boolean token to expression token
    auto tmp_elem = std::make_unique<psil_parser::token_elem_t>(std::move(tmp_bool));
    node->aspects.push_back( std::move( tmp_elem ) );
  }

  // Performs logical or on all arguments
  void psil_or( stack_ptr & s, token_ptr & node ) {
    //std::cout << "OR" << std::endl;
    size_t idx = 0;
    std::string val = "";
    //bool result = true;
    for ( auto itr = node->aspects.begin(); itr != node->aspects.end(); ++itr, ++idx ) {
      if ( idx > 1 && idx < node->aspects.size() - 1 ) { // Just arguments of function
	if ( is_true( s, (*itr)->tk ) ) {
	  val = "#t";
	  break;
	}
      }
    }
    if ( val.empty() ) { val = "#f"; }
    //std::cout << "OR: " << val << std::endl;
    node->type_name = "<constant>";
    node->aspects.clear();
    // create boolean token
    auto tmp_bool = std::make_unique<psil_parser::token_t>("<boolean>");
    // add value
    tmp_bool->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( val ) );
    // add boolean token to expression token
    auto tmp_elem = std::make_unique<psil_parser::token_elem_t>(std::move(tmp_bool));
    node->aspects.push_back( std::move( tmp_elem ) );
  }

  // Performs logical negation on all arguments
  void psil_not( stack_ptr & s, token_ptr & node ) {
    std::string val;
    if ( is_true( s, node->aspects[2]->tk ) ) {
      val = "#f";
    } else {
      val = "#t";
    }
    node->type_name = "<constant>";
    node->aspects.clear();
    // create boolean token
    auto tmp_bool = std::make_unique<psil_parser::token_t>("<boolean>");
    // add value
    tmp_bool->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( val ) );
    // add boolean token to expression token
    auto tmp_elem = std::make_unique<psil_parser::token_elem_t>(std::move(tmp_bool));
    node->aspects.push_back( std::move( tmp_elem ) );
  }

  // Checks the two arguments for equality
  void psil_is_equal( stack_ptr & s, token_ptr & node ) {}
  // Math
  // Operators
  // Addition of all numerical arguments
  void psil_add( stack_ptr & s, token_ptr & node ) {}
  // Subtraction of all the numerical arguments
  void psil_sub( stack_ptr & s, token_ptr & node ) {}
  // Multiplication of all the numerical arguments
  void psil_mult( stack_ptr & s, token_ptr & node ) {}
  // Division of all the numberical arguments
  void psil_div( stack_ptr & s, token_ptr & node ) {}
  // Finds the absolute value of the argument
  void psil_abs( stack_ptr & s, token_ptr & node ) {}
  // Finds the first argument mod the second argument
  void psil_mod( stack_ptr & s, token_ptr & node ) {}
  // Approx
  void psil_floor( stack_ptr & s, token_ptr & node ) {}
  void psil_ceil( stack_ptr & s, token_ptr & node ) {}
  void psil_trunc( stack_ptr & s, token_ptr & node ) {}
  void psil_round( stack_ptr & s, token_ptr & node ) {}
  // Inequalities
  void psil_lt( stack_ptr & s, token_ptr & node ) {}
  void psil_lte( stack_ptr & s, token_ptr & node ) {}
  void psil_gt( stack_ptr & s, token_ptr & node ) {}
  void psil_gte( stack_ptr & s, token_ptr & node ) {}
  void psil_eq( stack_ptr & s, token_ptr & node ) {}
  void psil_is_zero( stack_ptr & s, token_ptr & node ) {}
  // Character
  void psil_chlt( stack_ptr & s, token_ptr & node ) {}
  void psil_chlte( stack_ptr & s, token_ptr & node ) {}
  void psil_chgt( stack_ptr & s, token_ptr & node ) {}
  void psil_chgte( stack_ptr & s, token_ptr & node ) {}
  void psil_cheq( stack_ptr & s, token_ptr & node ) {}
  // List
  void psil_first( stack_ptr & s, token_ptr & node ) {}
  void psil_second( stack_ptr & s, token_ptr & node ) {}
  void psil_nth( stack_ptr & s, token_ptr & node ) {}
  void psil_set_first( stack_ptr & s, token_ptr & node ) {}
  void psil_set_second( stack_ptr & s, token_ptr & node ) {}
  void psil_set_nth( stack_ptr & s, token_ptr & node ) {}
  void psil_is_null( stack_ptr & s, token_ptr & node ) {}
  // Quote
  void psil_quote( stack_ptr & s, token_ptr & node ) {}
  void psil_unquote( stack_ptr & s, token_ptr & node ) {}
  // Identity predicates
  void psil_isbool( stack_ptr & s, token_ptr & node ) {}
  void psil_isnum( stack_ptr & s, token_ptr & node ) {}
  void psil_ischar( stack_ptr & s, token_ptr & node ) {}
  void psil_issymbol( stack_ptr & s, token_ptr & node ) {}
  void psil_isproc( stack_ptr & s, token_ptr & node ) {}
  void psil_islist( stack_ptr & s, token_ptr & node ) {}

}
