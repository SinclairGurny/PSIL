/**
   psil_exec_types.cpp
   PSIL Execution Library
   Type checks and
   Type conversion implementations
   @author Sinclair Gurny
   @version 1.0
   July 2019
*/

#include "psil_exec.h"

namespace psil_exec {

  // === If the node contains the type t, return true, else false
  void psil_type_check( token_ptr & node, VarType t ) {
    auto app = node->aspects.front()->tk.get();
    bool is_type = check_type( app->aspects[2]->tk ) == t;
    auto ret = make_boolean( is_type );
    node = std::move( ret );
  }


  // === If the node contains the type of number signified by int_or_dec return true
  void psil_num_check( token_ptr & node, bool int_or_dec ) {
    auto app = node->aspects.front()->tk.get();
    if ( check_type( app->aspects[2]->tk ) != VarType::NUM ) {
      throw std::string( "Number type check must be a number" );
    }

    //       <application>     <expression>        <constant>         <number>
    auto num = app->aspects[2]->tk->aspects.front()->tk->aspects.front()->tk.get();
    bool is_num_type;
    if ( int_or_dec ) is_num_type = num->aspects.front()->tk->type_name == "<integer>";
    else is_num_type = num->aspects.front()->tk->type_name == "<decimal>";
    
    auto ret = make_boolean( is_num_type );
    node = std::move( ret );
  }
  
}
