/**
   psil_exec_math.cpp
   PSIL Execution Library
   Global Math function implementations
   @author Sinclair Gurny
   @version 1.0
   July 2019
*/

#include "psil_exec.h"

namespace psil_exec {

  // === Helpers ===
  token_ptr make_number( std::string val, bool int_or_dec ) {
    // Make expression
    auto tmp_exp = std::make_unique<psil_parser::token_t>( "<expression>" );
    // Make constant
    auto tmp_con = std::make_unique<psil_parser::token_t>( "<constant>" );
    // Make number
    auto tmp_num = std::make_unique<psil_parser::token_t>( "<number>" );
    // Make int/dec
    std::string num_type = "<integer>";
    if ( !int_or_dec ) num_type = "<decimal>";
    auto tmp_int_dec = std::make_unique<psil_parser::token_t>( num_type );
    // Add val to integer
    if ( int_or_dec ) {
      try {
	val = std::to_string( std::stoll( val ) );
      } catch ( ... ) { std::string( "Conversion error" ); }
    }
    auto tmp_val = std::make_unique<psil_parser::token_elem_t>( val );
    tmp_int_dec->aspects.push_back( std::move( tmp_val ) );
    // Add integer to number
    auto elem_int_dec = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_int_dec ) );
    tmp_num->aspects.push_back( std::move( elem_int_dec ) );
    // Add number to const
    auto elem_num = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_num ) );
    tmp_con->aspects.push_back( std::move( elem_num ) );
    // Add const to expression
    auto elem_con = std::make_unique<psil_parser::token_elem_t>( std::move( tmp_con ) );
    tmp_exp->aspects.push_back( std::move( elem_con ) );
    
    return tmp_exp;
  }
  
  // ==================================== MATH ========================================================
  // Operators
  // Addition of all numerical arguments
  void psil_add( stack_ptr & s, token_ptr & node ) {
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
    std::string val = ( int_or_dec == 1 ) ? std::to_string( int_total ) : std::to_string( dec_total );
    auto number = make_number( val, int_or_dec == 1 );
    node = std::move( number );
  }
  
  // Subtraction of all the numerical arguments
  void psil_sub( stack_ptr & s, token_ptr & node ) {
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
    std::string val = ( int_or_dec == 1 ) ? std::to_string( int_total ) : std::to_string( dec_total );
    auto number = make_number( val, int_or_dec == 1 );
    node = std::move( number );
  }
  // Multiplication of all the numerical arguments
  void psil_mult( stack_ptr & s, token_ptr & node ) {
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
    std::string val = ( int_or_dec == 1 ) ? std::to_string( int_total ) : std::to_string( dec_total );
    auto number = make_number( val, int_or_dec == 1 );
    node = std::move( number );
  }
  
  // Division of all the numberical arguments
  void psil_div( stack_ptr & s, token_ptr & node ) {
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
    std::string val = std::to_string( dec_total );
    auto number = make_number( val, false );
    node = std::move( number );
  }


  void psil_round( token_ptr & node, std::function<long double(long double)> op ) {
    // Verify argument is number
    auto app = node->aspects.front()->tk.get();
    if ( check_type( app->aspects[2]->tk ) != VarType::NUM ) {
      throw std::string( "rounding procedure argument must be number" );
    }

    // Get number
    //          <expression>             <constant>            <number>
    auto num = app->aspects[2]->tk->aspects.front()->tk->aspects.front()->tk.get();
    long double tmp = 0;
    try {
      //             <number>              <int/dec>           value
      tmp = std::stold(num->aspects.front()->tk->aspects.front()->str);
      auto number = make_number( std::to_string( op( tmp )), true );
      node = std::move( number );
    } catch ( ... ) {
      throw std::string( "Number error" );
    }
    
  }
  
  // Finds the first argument mod the second argument
  void psil_mod( token_ptr & node ) {
    // Verify argument is number
    auto app = node->aspects.front()->tk.get();
    if ( check_type( app->aspects[2]->tk ) != VarType::NUM ||
	 check_type( app->aspects[3]->tk ) != VarType::NUM ) {
      throw std::string( "mod procedure arguments must be number" );
    }

    // Get number
    //          <expression>             <constant>            <number>
    auto num1 = app->aspects[2]->tk->aspects.front()->tk->aspects.front()->tk.get();
    auto num2 = app->aspects[3]->tk->aspects.front()->tk->aspects.front()->tk.get();
    long double arg1 = 0, arg2 = 0;
    try {
      //             <number>              <int/dec>           value
      arg1 = std::stoll(num1->aspects.front()->tk->aspects.front()->str);
      arg2 = std::stoll(num2->aspects.front()->tk->aspects.front()->str);
      auto number = make_number( std::to_string( remainder( arg1,  arg2 ) ), true );
      node = std::move( number );
    } catch ( ... ) {
      throw std::string( "Number error" );
    }
  }

}
