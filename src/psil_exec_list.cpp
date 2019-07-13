/**
   psil_exec_list.cpp
   PSIL Execution Library
   Global list function implementations
   @author Sinclair Gurny
   @version 0.5
   July 2019
*/

#include "psil_exec.h"

namespace psil_exec {

// List
  void psil_get_list( token_ptr & node, size_t pos ) {

    auto app = node->aspects.front()->tk.get();
    if ( check_type( app->aspects[2]->tk ) != VarType::LIST ) {
      throw std::string( "list operation procedure argument must be list" );
    }

    // Grab first element from list
    //        <expression>          <application>       <expression>
    auto arg_exp = node->aspects.front()->tk->aspects[2]->tk.get();
    //          <expression>             <constant>            <list_def>
    auto list_def = arg_exp->aspects.front()->tk->aspects.front()->tk.get();
    //         <list_def>            <datum>
    auto datum = list_def->aspects[2]->tk.get();
    //          <datum>               <list>
    auto list = datum->aspects.front()->tk.get();
    if ( pos > list->aspects.size() || list->aspects.size() - pos <= 2 ) {
      // List is empty
      throw std::string( "Out of bounds" );
    } else {
      auto elem = std::move( list->aspects[pos+1]->tk );
      elem->print();
      // Update datum
      list_def->aspects[2]->tk = std::move( elem );
      // Pull quote expression out
      auto quote = std::move( node->aspects.front()->tk->aspects[2]->tk );
      node = std::move( quote );
    }
  }
  
  void psil_set_list( token_ptr & node, size_t pos ) {
    // Check if first argument is a list
    //        <expression>          <application>
    auto app = node->aspects.front()->tk.get();
    if ( check_type( app->aspects[2]->tk ) != VarType::LIST ) {
      throw std::string( "list operation procedure argument 1 must be list" );
    }

    // Check if second argument is a quote
    //      <application>         <expression>     token_element
    auto arg2_elem = app->aspects[3]->tk->aspects.front().get();
    if ( arg2_elem->elem_type != TE_Type::TOKEN ||
	 arg2_elem->tk->type_name != "<constant>" ||
	 arg2_elem->tk->aspects.front()->tk->type_name != "<list_def>" ) {
      throw std::string( "list set operation procedure argument 2 must be quoted" );
    }

    // Pull out second argument
    //                     token_element     <constant>         <list_def>      <datum> 
    auto arg2_datum = std::move( arg2_elem->tk->aspects.front()->tk->aspects[2]->tk );
    
    // Grab first element from list
    //        <expression>          <application>      <expression>
    auto arg1_exp = node->aspects.front()->tk->aspects[2]->tk.get();
    //          <expression>             <constant>            <list_def>
    auto list_def = arg1_exp->aspects.front()->tk->aspects.front()->tk.get();
    //         <list_def>            <datum>
    auto datum = list_def->aspects[2]->tk.get();
    //          <datum>               <list>
    auto list = datum->aspects.front()->tk.get();
    if ( pos > list->aspects.size() || list->aspects.size() - pos <= 2 ) {
      throw std::string( "Out of bound" );
    } else {
      // Update list
      list->aspects[pos+1]->tk = std::move( arg2_datum );
      // Pull quote expression out
      auto quote = std::move( node->aspects.front()->tk->aspects[2]->tk );
      node = std::move( quote );
    }
    
  }
  void psil_get_nth( token_ptr & node ) {
    auto app = node->aspects.front()->tk.get();
    if ( check_type( app->aspects[3]->tk ) != VarType::NUM ) {
      throw std::string( "list operation procedure argument 2 must be number" );
    }

    //     <application>     <expression>         <constant>          <number>
    auto num = app->aspects[3]->tk->aspects.front()->tk->aspects.front()->tk.get();
    if ( num->aspects.front()->tk->type_name != "<integer>" ) {
      throw std::string( "Index must be integer" );
    }

    size_t pos = 0;
    try {
      //               <number>            <integer>            val
      pos = std::stoul( num->aspects.front()->tk->aspects.front()->str );
    } catch ( ... ) {
      throw std::string( "Number error" );
    }

    psil_get_list( node, pos );
  }
  void psil_set_nth( token_ptr & node ) {
    auto app = node->aspects.front()->tk.get();
    if ( check_type( app->aspects[3]->tk ) != VarType::NUM ) {
      throw std::string( "list operation procedure argument 2 must be number" );
    }

    //     <application>     <expression>         <constant>          <number>
    auto num = app->aspects[3]->tk->aspects.front()->tk->aspects.front()->tk.get();
    if ( num->aspects.front()->tk->type_name != "<integer>" ) {
      throw std::string( "Index must be integer" );
    }

    size_t pos = 0;
    try {
      //               <number>            <integer>            val
      pos = std::stoul( num->aspects.front()->tk->aspects.front()->str );
    } catch ( ... ) {
      throw std::string( "Number error" );
    }

    psil_set_list( node, pos );
  }
  void psil_is_null( token_ptr & node ) {}

  // =============== QUOTE =========================================
  void psil_quote( stack_ptr & s, token_ptr & node ) {
    // take structure and convert to datums
    // use parser?
  }
  void psil_unquote( stack_ptr & s, token_ptr & node ) {
    // take datums and convert to structure
    // use parser?
  }
}
