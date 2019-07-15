/**
   psil_exec_comp.cpp
   PSIL Execution Library
   Global Comparison Function Implementations
   @author Sinclair Gurny
   @version 0.9
   July 2019
*/

#include "psil_exec.h"

namespace psil_exec {

  long double psil_get_double( token_ptr & node ) {
    if ( check_type( node ) != VarType::NUM ) {
      throw std::string( "Not number" );
    }

    //       <expression>          <constant>           <number>
    auto num = node->aspects.front()->tk->aspects.front()->tk.get();
    //              <number>             <int/dec>             value
    std::string val = num->aspects.front()->tk->aspects.front()->str;
    if ( val.size() > 18 ) throw std::string( "Number overflow" );
    try {
      return stold( val );
    } catch ( ... ) {
      throw std::string( "Conversion error" );
    }
    return 0.0;
  }

  std::string psil_get_char( token_ptr & node ) {
    if ( check_type( node ) != VarType::CHAR ) {
      throw std::string( "Not character" );
    }

    //       <expression>          <constant>           <character>
    auto num = node->aspects.front()->tk->aspects.front()->tk.get();
    //              <character>           value
    return psil_char( num->aspects.front()->str );
  }
  
  // Inequalities
  void psil_num_compare( token_ptr & node,
			 std::function<bool(long double, long double)> comp ) {
    try {
      auto app = node->aspects.front()->tk.get();
      long double arg1 = psil_get_double( app->aspects[2]->tk );
      long double arg2 = psil_get_double( app->aspects[3]->tk );

      auto ret = make_boolean( comp( arg1, arg2 ) );
      node = std::move( ret );
    } catch ( ... ) {
      throw std::string( "Number conversion error" );
    }
  }
  
  void psil_is_zero( token_ptr & node ) {}
  // Character
  void psil_char_compare( token_ptr & node,
			 std::function<bool(std::string, std::string)> comp ) {
    try {
      auto app = node->aspects.front()->tk.get();
      std::string arg1 = psil_get_char( app->aspects[2]->tk );
      std::string arg2 = psil_get_char( app->aspects[3]->tk );

      auto ret = make_boolean( comp( arg1, arg2 ) );
      node = std::move( ret );
    } catch ( ... ) {
      throw std::string( "char comparison error" );
    }
  }
  
}
