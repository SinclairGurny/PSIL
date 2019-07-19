/**
   psil_exec_funcs.cpp
   PSIL Execution Library Global function implementations
   @author Sinclair Gurny
   @version 1.0
   July 2019
*/

#include "psil_exec.h"

namespace psil_exec {

  // === Take function name and apply correct function
  void apply_global_proc( stack_ptr & s, token_ptr & node, bool& rem, std::string fun ) {
    size_t arg_count = node->aspects.front()->tk->aspects.size()-3;
    // ==============================  Input / Output ===================================
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
    // ========================= Boolean operations  ================================
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
    // ========================= Arithmetic  =======================================
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
    // ============================ Approx  ======================================
    else if ( fun == "abs" ) {
      if ( arg_count != 1 )
	throw std::string( "abs: Wrong number of arguments given, 1 expected" );
      auto comp = [](long double a) -> long double { return abs(a); };
      psil_round( node, comp );
    } else if ( fun == "mod" ) {
      if ( arg_count != 2 )
	throw std::string( "mod: Wrong number of arguments given, 2 expected" );
      psil_mod( node );
    } else if ( fun == "floor" ) {
      if ( arg_count != 1 )
	throw std::string( "floor: Wrong number of arguments given, 1 expected" );
      auto comp = [](long double a) -> long double { return floor(a); };
      psil_round( node, comp );
    } else if ( fun == "ceil" ) {
      if ( arg_count != 1 )
	throw std::string( "ceil: Wrong number of arguments given, 1 expected" );
      auto comp = [](long double a) -> long double { return ceil(a); };
      psil_round( node, comp );
    } else if ( fun == "round" ) {
      if ( arg_count != 1 )
	throw std::string( "round: Wrong number of arguments given, 1 expected" );
      auto comp = [](long double a) -> long double { return round(a); };
      psil_round( node, comp );
    }
    // ========================== Inequalities  ================================
    else if ( fun == "lt" ) {
      if ( arg_count != 2 )
	throw std::string( "lt: Wrong number of arguments given, 2 expected" );
      auto comp = [](long double a, long double b) -> bool { return a < b; };
      psil_num_compare( node, comp );
    } else if ( fun == "lte" ) {
      if ( arg_count != 2 )
	throw std::string( "lte: Wrong number of arguments given, 2 expected" );
      auto comp = [](long double a, long double b) -> bool { return a <= b; };
      psil_num_compare( node, comp );
    } else if ( fun == "gt" ) {
      if ( arg_count != 2 )
	throw std::string( "gt: Wrong number of arguments given, 2 expected" );
      auto comp = [](long double a, long double b) -> bool { return a > b; };
      psil_num_compare( node, comp );
    } else if ( fun == "gte" ) {
      if ( arg_count != 2 )
	throw std::string( "gte: Wrong number of arguments given, 2 expected" );
      auto comp = [](long double a, long double b) -> bool { return a >= b; };
      psil_num_compare( node, comp );
    } else if ( fun == "eq" ) {
      if ( arg_count != 2 )
	throw std::string( "eq: Wrong number of arguments given, 2 expected" );
      auto comp = [](long double a, long double b) -> bool {
		    return (a-b) < 0.0000000001 && (a-b) > -0.0000000001;  };
      psil_num_compare( node, comp );
    }
    // ========================== Character  ===================================
    else if ( fun == "ch_lt" ) {
      if ( arg_count != 2 )
	throw std::string( "ch_lt: Wrong number of arguments given, 2 expected" );
      auto comp = [](std::string a, std::string b) -> bool { return a < b; };
      psil_char_compare( node, comp );
    } else if ( fun == "ch_lte" ) {
      if ( arg_count != 2 )
	throw std::string( "ch_lte: Wrong number of arguments given, 2 expected" );
      auto comp = [](std::string a, std::string b) -> bool { return a <= b; };
      psil_char_compare( node, comp );
    } else if ( fun == "ch_gt" ) {
      if ( arg_count != 2 )
	throw std::string( "ch_gt: Wrong number of arguments given, 2 expected" );
      auto comp = [](std::string a, std::string b) -> bool { return a > b; };
      psil_char_compare( node, comp );
    } else if ( fun == "ch_gte" ) {
      if ( arg_count != 2 )
	throw std::string( "ch_gte: Wrong number of arguments given, 2 expected" );
      auto comp = [](std::string a, std::string b) -> bool { return a >= b; };
      psil_char_compare( node, comp );
    } else if ( fun == "ch_eq" ) {
      if ( arg_count != 2 )
	throw std::string( "ch_eq: Wrong number of arguments given, 2 expected" );
      auto comp = [](std::string a, std::string b) -> bool { return a == b; };
      psil_char_compare( node, comp );
    }
    // =========================== List  ========================================
    else if ( fun == "length" ) {
      if ( arg_count != 1 )
	throw std::string( "length: Wront number of arguments given, 1 expected" );
      psil_length( node );
    } else if ( fun == "first" ) {
      if ( arg_count != 1 )
	throw std::string( "first: Wrong number of arguments given, 1 expected" );
      psil_get_list( node, 0 );
    } else if ( fun == "second" ) {
      if ( arg_count != 1 )
	throw std::string( "second: Wrong number of arguments given, 1 expected" );
      psil_get_list( node, 1 );
    } else if ( fun == "nth" ) {
      if ( arg_count != 2 )
	throw std::string( "nth: Wrong number of arguments given, 2 expected" );
      psil_get_nth( node );
    } else if ( fun == "first!" ) {
      if ( arg_count != 2 )
	throw std::string( "first!: Wrong number of arguments given, 2 expected" );
      psil_set_list( node, 0 );
    } else if ( fun == "second!" ) {
      if ( arg_count != 2 )
	throw std::string( "second: Wrong number of arguments given, 2 expected" );
      psil_set_list( node, 1 );
    } else if ( fun == "nth!" ) {
      if ( arg_count != 3 )
	throw std::string( "nth: Wrong number of arguments given, 3 expected" );
      psil_set_nth( node );
    } else if ( fun == "append" ) {
      if ( arg_count != 2 )
	throw std::string( "append: Wrong number of arguments given, 2 expected" );
      psil_append( node, -1 );
    } else if ( fun == "insert" ) {
      if ( arg_count != 3 )
	throw std::string( "insert: Wrong number of arguments given, 3 expected" );
      psil_insert( node );
    } else if ( fun == "pop" ) {
      if ( arg_count != 2 )
	throw std::string( "pop: Wrong number of arguments given, 2 expected" );
      psil_pop( node );
    } else if ( fun == "null?" ) {
      if ( arg_count != 1 )
	throw std::string( "null?: Wrong number of arguments given, 1 expected" );
      psil_is_null( node );
    } else if ( fun == "to_quote" ) {
      if ( arg_count != 1 )
	throw std::string( "to_quote: Wrong number of arguments given, 1 expected" );
      psil_quote( s, node );
    } else if ( fun == "unquote" ) {
      if ( arg_count != 1 )
	throw std::string( "unquote: Wrong number of arguments given, 1 expected" );
      psil_unquote( s, node );
    }
    // ======================= Identity  =========================================
    else if ( fun == "boolean?" ) {
      if ( arg_count != 1 )
	throw std::string( "boolean?: Wrong number of arguments given, 1 expected" );
      psil_type_check( node, VarType::BOOL );
    } else if ( fun == "number?" ) {
      if ( arg_count != 1 )
	throw std::string( "number?: Wrong number of arguments given, 1 expected" );
      psil_type_check( node, VarType::NUM );
    } else if ( fun == "character?" ) {
      if ( arg_count != 1 )
	throw std::string( "character?: Wrong number of arguments given, 1 expected" );
      psil_type_check( node, VarType::CHAR );
    } else if ( fun == "symbol?" ) {
      if ( arg_count != 1 )
	throw std::string( "symbol?: Wrong number of arguments given, 1 expected" );
      psil_type_check( node, VarType::SYMBOL );
    } else if ( fun == "proc?" ) {
      if ( arg_count != 1 )
	throw std::string( "proc?: Wrong number of arguments given, 1 expected" );
      psil_type_check( node, VarType::PROC );
    } else if ( fun == "list?" ) {
      if ( arg_count != 1 )
	throw std::string( "list?: Wrong number of arguments given, 1 expected" );
      psil_type_check( node, VarType::LIST );
    } else if ( fun == "integer?" ) {
      if ( arg_count != 1 )
	throw std::string( "integer?: Wrong number of arguments given, 1 expected" );
      psil_num_check( node, true );
    } else if ( fun == "decimal?" ) {
      if ( arg_count != 1 )
	throw std::string( "decimal?: Wrong number of arguments given, 1 expected" );
      psil_num_check( node, false );
    }
      
  }

  // ================= INPUT / OUTPUT ==========================================================

  // === Converts psil character to char
  std::string psil_char( std::string ch ) {
    if ( ch.size() > 3 ) {
      if ( ch == "#\\newline" ) {
	return "\n";
      } else if ( ch == "#\\space" ) {
	return " ";
      } else if ( ch == "#\\tab" ) {
	return "\t";
      } else if ( ch == "#\\oparen" ) {
	return "(";
      } else if ( ch == "#\\cparen" ) {
	return ")";
      } else if ( ch == "#\\osqbrac" ) {
	return "[";
      } else if ( ch == "#\\csqbrac" ) {
	return "]";
      }
    }
    return ch.substr(2);
  }

  // === Converts char to psil character
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

  // === Converts token back into printable string
  std::string tk_to_string( token_ptr & tk ) {
    if ( tk->type_name == "<constant>" || tk->type_name == "<datum>" ) {
      auto const_type = tk->aspects.front()->tk.get();
      if ( const_type->type_name == "<boolean>" ) {
	return const_type->aspects.front()->str + " ";
      } else if ( const_type->type_name == "<number>" ) {
	return const_type->aspects.front()->tk->aspects.front()->str + " ";
      } else if ( const_type->type_name == "<character>" ) {
	return psil_char( const_type->aspects.front()->str );
      } else if ( const_type->type_name == "<symbol>" ) {
	auto iden = const_type->aspects.front()->tk.get();
	if ( iden->aspects.front()->elem_type == TE_Type::TOKEN ) {
	  // Keyword or Operator
	  return iden->aspects.front()->tk->aspects.front()->str + " ";
	} else {
	  // Locally defined name
	  return iden->aspects.front()->str + " ";
	}
      } else if ( const_type->type_name == "<list_def>" ) {
	std::string ret = tk_to_string( const_type->aspects[2]->tk );
	return "'" + ret;
      } else if ( const_type->type_name == "<list>" ) {
	std::string ret = "";
	for ( auto itr = const_type->aspects.begin(); itr != const_type->aspects.end(); ++itr ) {
	  if ( (*itr)->elem_type == TE_Type::TOKEN ) {
	    ret += tk_to_string( (*itr)->tk );
	  }
	}
	return "( " + ret + ")";
      }
    } else {
      std::string ret = "";
      for ( auto itr = tk->aspects.begin(); itr != tk->aspects.end(); ++itr ) {
	if ( (*itr)->elem_type == TE_Type::TOKEN ) {
	  ret += tk_to_string( (*itr)->tk );
	}
      }
      return ret;
    }
    return "";
  }

  // === Prints constant types to string
  void print( token_ptr & node, bool newline ) {
    size_t idx = 0;
    for ( auto itr = node->aspects.begin(); itr != node->aspects.end(); ++itr, ++idx ) {
      if ( idx > 1 && idx < node->aspects.size() - 1 ) { // Just arguments of function call
	if ( (*itr)->elem_type == TE_Type::TOKEN && (*itr)->tk->aspects.size() == 1 &&
	     (*itr)->tk->aspects.front()->elem_type == TE_Type::TOKEN ) {
	  std::cout << tk_to_string( (*itr)->tk );
	}
      }
    }
    if ( newline )
      std::cout << std::endl;
  }

  // === Reads from cin, converts string to list or characters
  void psil_read( token_ptr & node ) {
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

    // === Convert string to (quote (<character>+))
    for ( char c : str ) {
      std::string val;
      val = psil_char( c );
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
