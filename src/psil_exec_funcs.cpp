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
    size_t arg_count = node->aspects.front()->tk->aspects.size()-3;
    // Input / Output
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
    // Boolean operations
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
    // Arithmetic
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
    // Approx
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
    // Inequalities
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
    // Character
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
    // List
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
    // Identity
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
      switch ( c ) {
      case ' ':
	val = "#\\space";
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

  // ========================= BOOLEAN OPERATIONS ================================================
  
  // Performs logical and on all arguments
  void psil_and( stack_ptr & s, token_ptr & node ) {
    std::cout << "AND" << std::endl;
    size_t idx = 0;
    std::string val = "";
    auto itr = node->aspects.front()->tk->aspects.begin();
    for ( ; itr != node->aspects.front()->tk->aspects.end(); ++itr, ++idx ) {
      if ( idx > 1 && idx < node->aspects.front()->tk->aspects.size() - 1 ) { // Just arguments of function
	if ( !is_true( s, (*itr)->tk ) ) {
	  val = "#f";
	  break;
	}
      }
    }
    if ( val.empty() ) { val = "#t"; }
    std::cout << "AND: " << val << std::endl;
    node->aspects.front()->tk->type_name = "<constant>";
    node->aspects.front()->tk->aspects.clear();
    // create boolean token
    auto tmp_bool = std::make_unique<psil_parser::token_t>("<boolean>");
    // add value
    tmp_bool->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( val ) );
    // add boolean token to expression token
    auto tmp_elem = std::make_unique<psil_parser::token_elem_t>(std::move(tmp_bool));
    node->aspects.front()->tk->aspects.push_back( std::move( tmp_elem ) );
  }

  // Performs logical or on all arguments
  void psil_or( stack_ptr & s, token_ptr & node ) {
    //std::cout << "OR" << std::endl;
    size_t idx = 0;
    std::string val = "";
    auto itr = node->aspects.front()->tk->aspects.begin();
    for ( ; itr != node->aspects.front()->tk->aspects.end(); ++itr, ++idx ) {
      if ( idx > 1 && idx < node->aspects.front()->tk->aspects.size() - 1 ) { // Just arguments of function
	if ( is_true( s, (*itr)->tk ) ) {
	  val = "#t";
	  break;
	}
      }
    }
    if ( val.empty() ) { val = "#f"; }
    //std::cout << "OR: " << val << std::endl;
    node->aspects.front()->tk->type_name = "<constant>";
    node->aspects.front()->tk->aspects.clear();
    // create boolean token
    auto tmp_bool = std::make_unique<psil_parser::token_t>("<boolean>");
    // add value
    tmp_bool->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( val ) );
    // add boolean token to expression token
    auto tmp_elem = std::make_unique<psil_parser::token_elem_t>(std::move(tmp_bool));
    node->aspects.front()->tk->aspects.push_back( std::move( tmp_elem ) );
  }

  // Performs logical negation on all arguments
  void psil_not( stack_ptr & s, token_ptr & node ) {
    std::string val;
    if ( is_true( s, node->aspects.front()->tk->aspects[2]->tk ) ) {
      val = "#f";
    } else {
      val = "#t";
    }
    node->aspects.front()->tk->type_name = "<constant>";
    node->aspects.front()->tk->aspects.clear();
    // create boolean token
    auto tmp_bool = std::make_unique<psil_parser::token_t>("<boolean>");
    // add value
    tmp_bool->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( val ) );
    // add boolean token to expression token
    auto tmp_elem = std::make_unique<psil_parser::token_elem_t>(std::move(tmp_bool));
    node->aspects.front()->tk->aspects.push_back( std::move( tmp_elem ) );
  }

  // Checks the two arguments for equality
  void psil_is_equal( stack_ptr & s, token_ptr & node ) {
    std::string val;
    auto app = node->aspects.front()->tk.get();
    if ( equal_tk( app->aspects[2]->tk, app->aspects[3]->tk ) ) {
      val = "#t";
    } else {
      val = "#f";
    }
    
    node->aspects.front()->tk->type_name = "<constant>";
    node->aspects.front()->tk->aspects.clear();
    // create boolean token
    auto tmp_bool = std::make_unique<psil_parser::token_t>("<boolean>");
    // add value
    tmp_bool->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( val ) );
    // add boolean token to expression token
    auto tmp_elem = std::make_unique<psil_parser::token_elem_t>( std::move(tmp_bool) );
    node->aspects.front()->tk->aspects.push_back( std::move( tmp_elem ) );
  }
  // ==================================== MATH ========================================================
  // Operators
  // Addition of all numerical arguments
  void psil_add( stack_ptr & s, token_ptr & node ) {
    std::cout << "ADD" << std::endl;
    size_t idx = 0;
    int int_or_dec = 0; // Determines type of result
    long long int_total = 0; 
    long double dec_total = 0.0;
    //      <expression>        <application>
    auto itr = node->aspects.front()->tk->aspects.begin();
    for ( ; itr != node->aspects.front()->tk->aspects.end(); ++itr, ++idx ) {
      if ( idx > 1 && idx < node->aspects.front()->tk->aspects.size() - 1 ) { // Just arguments of function
	if ( (*itr)->tk->aspects.front()->tk->type_name != "<constant>" ||
	     (*itr)->tk->aspects.front()->tk->aspects.front()->tk->type_name != "<number>" ) {
	  throw std::string( "Operation expects numbers" );
	}
	
	//        <expression>               <constant>           <number>
	auto num = (*itr)->tk->aspects.front()->tk->aspects.front()->tk.get();
	std::string num_type = num->aspects.front()->tk->type_name;
	std::cout << " here " << num_type << std::endl;
	if ( num_type == "<integer>" ) {
	  if ( int_or_dec == 0 || int_or_dec == 1 ) {
	    int_or_dec = 1;
	    long long tmp = 0;
	    try { // Convert string to long long
	      tmp = std::stoll(num->aspects.front()->tk->aspects.front()->str);
	    } catch ( ... ) {
	      throw std::string( "Number error" );
	    }
	    int_total += tmp;
	  } else { // Add to dec
	    long double tmp = 0;
	    try { // Convert string to long long
	      tmp = std::stold(num->aspects.front()->tk->aspects.front()->str);
	    } catch ( ... ) {
	      throw std::string( "Number error" );
	    }
	    dec_total += tmp;
	  }
	} else {
	  if ( int_or_dec == 0 || int_or_dec == 1 ) {
	    // Convert from int to dec
	    int_or_dec = 2;
	    long double tmp = 0;
	    try { // Convert string to long long
	      tmp = std::stold(num->aspects.front()->tk->aspects.front()->str);
	      tmp += std::stold( std::to_string( int_total ) ); // TODO
	    } catch ( ... ) {
	      throw std::string( "Number error" );
	    }
	    dec_total += tmp;
	  } else { // Keep using dec
	    long double tmp = 0;
	    try { // Convert string to long long
	      tmp = std::stold(num->aspects.front()->tk->aspects.front()->str);
	    } catch ( ... ) {
	      throw std::string( "Number error" );
	    }
	    dec_total += tmp;
	  }
	}
      }
    }
    if ( int_or_dec == 0 ) throw std::string( "Argument error" );
    // Reset application to be constant
    node->aspects.front()->tk->type_name = "<constant>";
    node->aspects.front()->tk->aspects.clear();
    // Make number
    auto tmp_num = std::make_unique<psil_parser::token_t>( "<number>" );
    // Make integer or decimal
    std::string _int_dec = ( int_or_dec == 1 ) ? "<integer>" : "<decimal>";
    auto tmp_int_dec = std::make_unique<psil_parser::token_t>( _int_dec );
    // Add val to int/dec
    std::string val = ( int_or_dec == 1 ) ? std::to_string( int_total ) : std::to_string( dec_total );
    auto tmp_val = std::make_unique<psil_parser::token_elem_t>( val );
    tmp_int_dec->aspects.push_back( std::move( tmp_val ) );
    // Add int/dec to number
    auto elem_int_dec = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_int_dec ) );
    tmp_num->aspects.push_back( std::move( elem_int_dec ) );
    // Add num to const
    auto elem_num = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_num ) );
    node->aspects.front()->tk->aspects.push_back( std::move( elem_num ) );
  }
  
  // Subtraction of all the numerical arguments
  void psil_sub( stack_ptr & s, token_ptr & node ) {
    std::cout << "SUB" << std::endl;
    size_t idx = 0;
    int int_or_dec = 0; // Determines type of result
    long long int_total = 0; 
    long double dec_total = 0.0;
    //      <expression>        <application>
    auto itr = node->aspects.front()->tk->aspects.begin();
    for ( ; itr != node->aspects.front()->tk->aspects.end(); ++itr, ++idx ) {
      if ( idx > 1 && idx < node->aspects.front()->tk->aspects.size() - 1 ) { // Just arguments of function
	if ( (*itr)->tk->aspects.front()->tk->type_name != "<constant>" ||
	     (*itr)->tk->aspects.front()->tk->aspects.front()->tk->type_name != "<number>" ) {
	  throw std::string( "Operation expects numbers" );
	}
	
	//        <expression>               <constant>           <number>
	auto num = (*itr)->tk->aspects.front()->tk->aspects.front()->tk.get();
	std::string num_type = num->aspects.front()->tk->type_name;
	std::cout << " here " << num_type << std::endl;
	if ( num_type == "<integer>" ) {
	  if ( int_or_dec == 0 || int_or_dec == 1 ) {
	    long long tmp = 0;
	    try { // Convert string to long long
	      tmp = std::stoll(num->aspects.front()->tk->aspects.front()->str);
	    } catch ( ... ) {
	      throw std::string( "Number error" );
	    }
	    if ( int_or_dec == 0 ) {
	      int_total = tmp;
	    } else {
	      int_total -= tmp;
	    }
	    int_or_dec = 1;
	  } else { // Add to dec
	    long double tmp = 0;
	    try { // Convert string to long long
	      tmp = std::stold(num->aspects.front()->tk->aspects.front()->str);
	    } catch ( ... ) {
	      throw std::string( "Number error" );
	    }
	    dec_total -= tmp;
	  }
	} else {
	  if ( int_or_dec == 0 || int_or_dec == 1 ) {
	    // Convert from int to dec
	    long double tmp = 0;
	    try { // Convert string to long long
	      if ( int_or_dec == 0 ) {
		tmp = std::stold(num->aspects.front()->tk->aspects.front()->str);
	      } else {
		tmp = std::stold( std::to_string( int_total ) ); // TODO
		tmp -= std::stold(num->aspects.front()->tk->aspects.front()->str);
	      }
	    } catch ( ... ) {
	      throw std::string( "Number error" );
	    }
	    dec_total = tmp;
	    int_or_dec = 2;
	  } else { // Keep using dec
	    long double tmp = 0;
	    try { // Convert string to long long
	      tmp = std::stold(num->aspects.front()->tk->aspects.front()->str);
	    } catch ( ... ) {
	      throw std::string( "Number error" );
	    }
	    dec_total -= tmp;
	  }
	}
      }
    }
    if ( int_or_dec == 0 ) throw std::string( "Argument error" );
    // Reset application to be constant
    node->aspects.front()->tk->type_name = "<constant>";
    node->aspects.front()->tk->aspects.clear();
    // Make number
    auto tmp_num = std::make_unique<psil_parser::token_t>( "<number>" );
    // Make integer or decimal
    std::string _int_dec = ( int_or_dec == 1 ) ? "<integer>" : "<decimal>";
    auto tmp_int_dec = std::make_unique<psil_parser::token_t>( _int_dec );
    // Add val to int/dec
    std::string val = ( int_or_dec == 1 ) ? std::to_string( int_total ) : std::to_string( dec_total );
    auto tmp_val = std::make_unique<psil_parser::token_elem_t>( val );
    tmp_int_dec->aspects.push_back( std::move( tmp_val ) );
    // Add int/dec to number
    auto elem_int_dec = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_int_dec ) );
    tmp_num->aspects.push_back( std::move( elem_int_dec ) );
    // Add num to const
    auto elem_num = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_num ) );
    node->aspects.front()->tk->aspects.push_back( std::move( elem_num ) );

  }
  // Multiplication of all the numerical arguments
  void psil_mult( stack_ptr & s, token_ptr & node ) {
        std::cout << "MULT" << std::endl;
    size_t idx = 0;
    int int_or_dec = 0; // Determines type of result
    long long int_total = 1; 
    long double dec_total = 1.0;
    //      <expression>        <application>
    auto itr = node->aspects.front()->tk->aspects.begin();
    for ( ; itr != node->aspects.front()->tk->aspects.end(); ++itr, ++idx ) {
      if ( idx > 1 && idx < node->aspects.front()->tk->aspects.size() - 1 ) { // Just arguments of function
	if ( (*itr)->tk->aspects.front()->tk->type_name != "<constant>" ||
	     (*itr)->tk->aspects.front()->tk->aspects.front()->tk->type_name != "<number>" ) {
	  throw std::string( "Operation expects numbers" );
	}
	
	//        <expression>               <constant>           <number>
	auto num = (*itr)->tk->aspects.front()->tk->aspects.front()->tk.get();
	std::string num_type = num->aspects.front()->tk->type_name;
	std::cout << " here " << num_type << std::endl;
	if ( num_type == "<integer>" ) {
	  if ( int_or_dec == 0 || int_or_dec == 1 ) {
	    int_or_dec = 1;
	    long long tmp = 0;
	    try { // Convert string to long long
	      tmp = std::stoll(num->aspects.front()->tk->aspects.front()->str);
	    } catch ( ... ) {
	      throw std::string( "Number error" );
	    }
	    int_total *= tmp;
	  } else { // Add to dec
	    long double tmp = 0;
	    try { // Convert string to long long
	      tmp = std::stold(num->aspects.front()->tk->aspects.front()->str);
	    } catch ( ... ) {
	      throw std::string( "Number error" );
	    }
	    dec_total *= tmp;
	  }
	} else {
	  if ( int_or_dec == 0 || int_or_dec == 1 ) {
	    // Convert from int to dec
	    int_or_dec = 2;
	    long double tmp = 0;
	    try { // Convert string to long long
	      tmp = std::stold(num->aspects.front()->tk->aspects.front()->str);
	      tmp *= std::stold( std::to_string( int_total ) ); // TODO
	    } catch ( ... ) {
	      throw std::string( "Number error" );
	    }
	    dec_total *= tmp;
	  } else { // Keep using dec
	    long double tmp = 0;
	    try { // Convert string to long long
	      tmp = std::stold(num->aspects.front()->tk->aspects.front()->str);
	    } catch ( ... ) {
	      throw std::string( "Number error" );
	    }
	    dec_total *= tmp;
	  }
	}
      }
    }
    if ( int_or_dec == 0 ) throw std::string( "Argument error" );
    // Reset application to be constant
    node->aspects.front()->tk->type_name = "<constant>";
    node->aspects.front()->tk->aspects.clear();
    // Make number
    auto tmp_num = std::make_unique<psil_parser::token_t>( "<number>" );
    // Make integer or decimal
    std::string _int_dec = ( int_or_dec == 1 ) ? "<integer>" : "<decimal>";
    auto tmp_int_dec = std::make_unique<psil_parser::token_t>( _int_dec );
    // Add val to int/dec
    std::string val = ( int_or_dec == 1 ) ? std::to_string( int_total ) : std::to_string( dec_total );
    auto tmp_val = std::make_unique<psil_parser::token_elem_t>( val );
    tmp_int_dec->aspects.push_back( std::move( tmp_val ) );
    // Add int/dec to number
    auto elem_int_dec = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_int_dec ) );
    tmp_num->aspects.push_back( std::move( elem_int_dec ) );
    // Add num to const
    auto elem_num = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_num ) );
    node->aspects.front()->tk->aspects.push_back( std::move( elem_num ) );

  }
  // Division of all the numberical arguments
  void psil_div( stack_ptr & s, token_ptr & node ) {
    std::cout << "DIV" << std::endl;
    size_t idx = 0;
    bool first = true;
    long double dec_total = 0.0;
    //      <expression>        <application>
    auto itr = node->aspects.front()->tk->aspects.begin();
    for ( ; itr != node->aspects.front()->tk->aspects.end(); ++itr, ++idx ) {
      if ( idx > 1 && idx < node->aspects.front()->tk->aspects.size() - 1 ) { // Just arguments of function
	if ( (*itr)->tk->aspects.front()->tk->type_name != "<constant>" ||
	     (*itr)->tk->aspects.front()->tk->aspects.front()->tk->type_name != "<number>" ) {
	  throw std::string( "Operation expects numbers" );
	}
	
	//        <expression>               <constant>           <number>
	auto num = (*itr)->tk->aspects.front()->tk->aspects.front()->tk.get();
	std::string num_type = num->aspects.front()->tk->type_name;
	std::cout << " here " << num_type << std::endl;
	long double tmp = 0;
	try { // Convert string to long long
	  tmp = std::stold(num->aspects.front()->tk->aspects.front()->str);
	} catch ( ... ) {
	  throw std::string( "Number error" );
	}
	if ( first ) {
	  dec_total = tmp;
	  first = false;
	} else {
	  dec_total /= tmp;
	}
      }
    }
    if ( first ) { throw std::string( "Math error" ); }
    // Reset application to be constant
    node->aspects.front()->tk->type_name = "<constant>";
    node->aspects.front()->tk->aspects.clear();
    // Make number
    auto tmp_num = std::make_unique<psil_parser::token_t>( "<number>" );
    // Make integer or decimal
    auto tmp_int_dec = std::make_unique<psil_parser::token_t>( "<decimal>" );
    // Add val to int/dec
    std::string val = std::to_string( dec_total );
    auto tmp_val = std::make_unique<psil_parser::token_elem_t>( val );
    tmp_int_dec->aspects.push_back( std::move( tmp_val ) );
    // Add int/dec to number
    auto elem_int_dec = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_int_dec ) );
    tmp_num->aspects.push_back( std::move( elem_int_dec ) );
    // Add num to const
    auto elem_num = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_num ) );
    node->aspects.front()->tk->aspects.push_back( std::move( elem_num ) );

  }
  // Finds the absolute value of the argument
  void psil_abs( stack_ptr & s, token_ptr & node ) {}
  // Finds the first argument mod the second argument
  void psil_mod( stack_ptr & s, token_ptr & node ) {
    // Must be integers
  }
  // Approx
  void psil_floor( stack_ptr & s, token_ptr & node ) {
    // return same if integer,
    // round down otherwise
  }
  void psil_ceil( stack_ptr & s, token_ptr & node ) {}
  void psil_trunc( stack_ptr & s, token_ptr & node ) {}
  void psil_round( stack_ptr & s, token_ptr & node ) {}
  // Inequalities
  // TODO - write these by taking a lambda comparator
  void psil_lt( stack_ptr & s, token_ptr & node ) {}
  void psil_lte( stack_ptr & s, token_ptr & node ) {}
  void psil_gt( stack_ptr & s, token_ptr & node ) {}
  void psil_gte( stack_ptr & s, token_ptr & node ) {}
  void psil_eq( stack_ptr & s, token_ptr & node ) {}
  void psil_is_zero( stack_ptr & s, token_ptr & node ) {}
  // Character
  // TODO - write these by taking a lambda comparator
  void psil_ch_lt( stack_ptr & s, token_ptr & node ) {}
  void psil_ch_lte( stack_ptr & s, token_ptr & node ) {}
  void psil_ch_gt( stack_ptr & s, token_ptr & node ) {}
  void psil_ch_gte( stack_ptr & s, token_ptr & node ) {}
  void psil_ch_eq( stack_ptr & s, token_ptr & node ) {}
  // List
  void psil_first( stack_ptr & s, token_ptr & node ) {
    // pull first value from list
  }
  void psil_second( stack_ptr & s, token_ptr & node ) {}
  void psil_nth( stack_ptr & s, token_ptr & node ) {}
  void psil_set_first( stack_ptr & s, token_ptr & node ) {}
  void psil_set_second( stack_ptr & s, token_ptr & node ) {}
  void psil_set_nth( stack_ptr & s, token_ptr & node ) {}
  void psil_is_null( stack_ptr & s, token_ptr & node ) {}
  // Quote
  void psil_quote( stack_ptr & s, token_ptr & node ) {
    //?
  }
  void psil_unquote( stack_ptr & s, token_ptr & node ) {
    // remove quote from list
  }
  // Identity predicates
  void psil_isbool( stack_ptr & s, token_ptr & node ) {
    // call check type?
  }
  void psil_isnum( stack_ptr & s, token_ptr & node ) {}
  void psil_ischar( stack_ptr & s, token_ptr & node ) {}
  void psil_issymbol( stack_ptr & s, token_ptr & node ) {}
  void psil_isproc( stack_ptr & s, token_ptr & node ) {}
  void psil_islist( stack_ptr & s, token_ptr & node ) {}

}
