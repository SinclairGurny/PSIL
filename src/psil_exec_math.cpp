/**
   psil_exec_math.cpp
   PSIL Execution Library
   Global Math function implementations
   @author Sinclair Gurny
   @version 0.5
   July 2019
*/

#include "psil_exec.h"

namespace psil_exec {

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


}
