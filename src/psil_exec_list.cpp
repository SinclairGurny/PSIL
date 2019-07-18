/**
   psil_exec_list.cpp
   PSIL Execution Library
   Global list function implementations
   @author Sinclair Gurny
   @version 1.0
   July 2019
*/

#include "psil_exec.h"

namespace psil_exec {

  // ============= List ==============================================

  void psil_length( token_ptr & node ) {
    // Verify that argument is list
    auto app = node->aspects.front()->tk.get();
    if ( check_type( app->aspects[2]->tk ) != VarType::LIST ) {
      throw std::string( "list operation procedure argument must be list" );
    }

    // Get reference to list
    //        <expression>          <application>       <expression>
    auto arg_exp = node->aspects.front()->tk->aspects[2]->tk.get();
    //          <expression>             <constant>            <list_def>
    auto list_def = arg_exp->aspects.front()->tk->aspects.front()->tk.get();
    //         <list_def>            <datum>
    auto datum = list_def->aspects[2]->tk.get();
    //          <datum>               <list>
    auto list = datum->aspects.front()->tk.get();
    size_t len = list->aspects.size() - 2;

    // Return result
    auto num = make_number( std::to_string( len ), true );
    node = std::move( num );
  }
  
  void psil_get_list( token_ptr & node, long pos ) {
    // Verify that argument is list
    auto app = node->aspects.front()->tk.get();
    if ( check_type( app->aspects[2]->tk ) != VarType::LIST ) {
      throw std::string( "list operation procedure argument must be list" );
    }

    // Get reference to list
    //        <expression>          <application>       <expression>
    auto arg_exp = node->aspects.front()->tk->aspects[2]->tk.get();
    //          <expression>             <constant>            <list_def>
    auto list_def = arg_exp->aspects.front()->tk->aspects.front()->tk.get();
    //         <list_def>            <datum>
    auto datum = list_def->aspects[2]->tk.get();
    //          <datum>               <list>
    auto list = datum->aspects.front()->tk.get();
    // Grab element if possible
    long len = list->aspects.size()-2;
    if ( pos < 0 ) {
      pos = len + pos + 1;
    }
    if ( pos < 0 || pos >= len ) {
      throw std::string( "Out of bounds" );
    } else {
      auto elem = std::move( list->aspects[pos+1]->tk );
      // Update datum
      list_def->aspects[2]->tk = std::move( elem );
      // Return updated list
      auto quote = std::move( node->aspects.front()->tk->aspects[2]->tk );
      node = std::move( quote );
    }
  }
  
  void psil_set_list( token_ptr & node, long pos ) {
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
    
    // Get reference to list
    //        <expression>          <application>      <expression>
    auto arg1_exp = node->aspects.front()->tk->aspects[2]->tk.get();
    //          <expression>             <constant>            <list_def>
    auto list_def = arg1_exp->aspects.front()->tk->aspects.front()->tk.get();
    //         <list_def>            <datum>
    auto datum = list_def->aspects[2]->tk.get();
    //          <datum>               <list>
    auto list = datum->aspects.front()->tk.get();
    // Check for bounds
    long len = list->aspects.size()-2;
    if ( pos < 0 ) {
      pos = len + pos + 1;
    }
    if ( pos < 0 || pos >= len ) {
      throw std::string( "Out of bound" );
    } else {
      // Update list
      list->aspects[pos+1]->tk = std::move( arg2_datum );
      // Return updated list
      auto quote = std::move( node->aspects.front()->tk->aspects[2]->tk );
      node = std::move( quote );
    }
    
  }
  void psil_get_nth( token_ptr & node ) {
    // Check if second argument is a number
    auto app = node->aspects.front()->tk.get();
    if ( check_type( app->aspects[3]->tk ) != VarType::NUM ) {
      throw std::string( "list operation procedure argument 2 must be number" );
    }

    // Convert argument into integer value
    //     <application>     <expression>         <constant>          <number>
    auto num = app->aspects[3]->tk->aspects.front()->tk->aspects.front()->tk.get();
    if ( num->aspects.front()->tk->type_name != "<integer>" ) {
      throw std::string( "Index must be integer" );
    }

    long pos = 0;
    try {
      //               <number>            <integer>            val
      pos = std::stol( num->aspects.front()->tk->aspects.front()->str );
    } catch ( ... ) {
      throw std::string( "Number error" );
    }

    // Perform get operation
    psil_get_list( node, pos );
  }
  void psil_set_nth( token_ptr & node ) {
    // Verify third argument is a number
    auto app = node->aspects.front()->tk.get();
    if ( check_type( app->aspects[4]->tk ) != VarType::NUM ) {
      throw std::string( "list operation procedure argument 3 must be number" );
    }

    // Convert argument into integer value
    //     <application>     <expression>         <constant>          <number>
    auto num = app->aspects[4]->tk->aspects.front()->tk->aspects.front()->tk.get();
    if ( num->aspects.front()->tk->type_name != "<integer>" ) {
      throw std::string( "Index must be integer" );
    }

    long pos = 0;
    try {
      //               <number>            <integer>            val
      pos = std::stoll( num->aspects.front()->tk->aspects.front()->str );
    } catch ( ... ) {
      throw std::string( "Number error" );
    }

    // Perform set operation
    psil_set_list( node, pos );
  }
  
  void psil_append( token_ptr & node, long location ) {
    // First argument is a list
    auto app = node->aspects.front()->tk.get();
    if ( check_type( app->aspects[2]->tk ) != VarType::LIST ) {
      throw std::string( "list operation procedure argument must be list" );
    }

    // Check if second argument is quoted
    //      <application>         <expression>     token_element
    auto arg2_elem = app->aspects[3]->tk->aspects.front().get();
    if ( arg2_elem->elem_type != TE_Type::TOKEN ||
	 arg2_elem->tk->type_name != "<constant>" ||
	 arg2_elem->tk->aspects.front()->tk->type_name != "<list_def>" ) {
      throw std::string( "list set operation procedure argument 2 must be quoted" );
    }

    // Pull out second argument
    //                     token_element     <constant>         <list_def>      <datum> 
    auto arg2_datum = std::move( arg2_elem->tk->aspects.front()->tk->aspects[2] );

    // Grab element from list
    //        <expression>          <application>       <expression>
    auto arg_exp = node->aspects.front()->tk->aspects[2]->tk.get();
    //          <expression>             <constant>            <list_def>
    auto list_def = arg_exp->aspects.front()->tk->aspects.front()->tk.get();
    //         <list_def>            <datum>
    auto datum = list_def->aspects[2]->tk.get();
    //          <datum>               <list>
    auto list = datum->aspects.front()->tk.get();

    // Find location to insert
    auto itr = list->aspects.begin();
    long pos = 0, list_len = list->aspects.size() - 2;
    if ( location >= 0 ) {
      pos = location;
    } else {
      pos = list_len + location + 1;
    }
    if ( pos < 0 || pos > list_len ) {
      throw std::string( "Index out of bounds" );
    }
    std::advance( itr, pos+1 );

    // Insert datum
    list->aspects.insert( itr, std::move( arg2_datum ) );

    // Return updated list
    auto quote = std::move( node->aspects.front()->tk->aspects[2]->tk );
    node = std::move( quote );
  }

  void psil_insert( token_ptr & node ) {
    // Verifty third argument is number
    auto app = node->aspects.front()->tk.get();
    if ( check_type( app->aspects[4]->tk ) != VarType::NUM ) {
      throw std::string( "list operation procedure argument 2 must be number" );
    }

    // Convert argument into integer value
    //     <application>     <expression>         <constant>          <number>
    auto num = app->aspects[4]->tk->aspects.front()->tk->aspects.front()->tk.get();
    if ( num->aspects.front()->tk->type_name != "<integer>" ) {
      throw std::string( "Index must be integer" );
    }

    long pos = 0;
    try {
      //               <number>            <integer>            val
      pos = std::stol( num->aspects.front()->tk->aspects.front()->str );
    } catch ( ... ) {
      throw std::string( "Number error" );
    }

    // Perform append operation
    psil_append( node, pos );
  }

  void psil_pop( token_ptr & node ) {
    // First argument is a list
    auto app = node->aspects.front()->tk.get();
    if ( check_type( app->aspects[2]->tk ) != VarType::LIST ) {
      throw std::string( "list operation procedure argument must be list" );
    }

    // Second argument is a number
    if ( check_type( app->aspects[3]->tk ) != VarType::NUM ) {
      throw std::string( "list operation procedure argument 2 must be number" );
    }
    // Convert argument to a integer value
    //     <application>     <expression>         <constant>          <number>
    auto num = app->aspects[3]->tk->aspects.front()->tk->aspects.front()->tk.get();
    if ( num->aspects.front()->tk->type_name != "<integer>" ) {
      throw std::string( "Index must be integer" );
    }

    long arg_val = 0;
    try {
      //               <number>            <integer>            val
      arg_val = std::stol( num->aspects.front()->tk->aspects.front()->str );
    } catch ( ... ) {
      throw std::string( "Number error" );
    }

    // Grab element from list
    //        <expression>          <application>       <expression>
    auto arg_exp = node->aspects.front()->tk->aspects[2]->tk.get();
    //          <expression>             <constant>            <list_def>
    auto list_def = arg_exp->aspects.front()->tk->aspects.front()->tk.get();
    //         <list_def>            <datum>
    auto datum = list_def->aspects[2]->tk.get();
    //          <datum>               <list>
    auto list = datum->aspects.front()->tk.get();

    // Find location to pop
    auto itr = list->aspects.begin();
    long pos = 0, list_len = list->aspects.size() - 2;
    if ( arg_val >= 0 ) {
      pos = arg_val;
    } else {
      pos = list_len + arg_val + 1;
    }
    if ( pos < 0 || pos > list_len ) {
      throw std::string( "Index out of bounds" );
    }
    std::advance( itr, pos+1 );
      
    list->aspects.erase( itr );

    auto quote = std::move( node->aspects.front()->tk->aspects[2]->tk );
    node = std::move( quote );
  }

  void psil_is_null( token_ptr & node ) {
    auto app = node->aspects.front()->tk.get();
    if ( check_type( app->aspects[2]->tk ) != VarType::LIST ) {
      throw std::string( "list operation procedure argument must be list" );
    }

    //        <expression>          <application>       <expression>
    auto arg_exp = node->aspects.front()->tk->aspects[2]->tk.get();
    //          <expression>             <constant>            <list_def>
    auto list_def = arg_exp->aspects.front()->tk->aspects.front()->tk.get();
    //         <list_def>            <datum>
    auto datum = list_def->aspects[2]->tk.get();
    //          <datum>               <list>
    auto list = datum->aspects.front()->tk.get();

    auto ret = make_boolean( list->aspects.size()-2 == 0 );

    node = std::move( ret );
  }

  // =============== QUOTE =========================================
  // Convert expressions into datums
  void psil_quote( stack_ptr & s, token_ptr & node ) { // TODO remove s
    // === Convert datum into code string ===
    auto app = node->aspects.front()->tk.get();
    //            <application>    arg <exp..>         element
    auto arg_elem = app->aspects[2]->tk->aspects.front().get();
    std::string datum_code = arg_elem->tk->to_code();
    // Quote expression
    //datum_code = "(quote " + datum_code + ")";
    //std::cout << datum_code << std::endl;

    // === UNQUOTE ===
    try {
      // Remake PSIL
      auto lang = psil_parser::make_psil_lang();
      // Parse code
      auto ast = psil_parser::parse( lang, datum_code );
      if ( !ast ) { throw 1; } // Error while parsing
      // DEBUG
      //ast->print();
      //std::cout << ast->to_code() << std::endl;
      
      // === Run eval library to check for error ===
      bool e = psil_eval::check_node( ast.get() );
      if ( !e ) { throw 1; } // Error while evaluating
      
      // === Take result and update AST ===
      //       <program>            <form>               <expression>
      auto tmp = std::move( ast->aspects.front()->tk->aspects.front()->tk );
      node = std::move( tmp );
    } catch ( ... ) {
      throw std::string( "Error while unquoting" );
    }
  }

  // Convert datums into expressions
  void psil_unquote( stack_ptr & s, token_ptr & node ) {
    // === Verify argument is correct type ===
    auto app = node->aspects.front()->tk.get();
    auto arg_elem = app->aspects[2]->tk->aspects.front().get();
    if ( arg_elem->elem_type != TE_Type::TOKEN ||
	 arg_elem->tk->type_name != "<constant>" ||
	 arg_elem->tk->aspects.front()->tk->type_name != "<list_def>" ) {
      throw std::string( "unquote argument must be quoted" );
    }

    // === Convert datum into code string ===
    auto qt_arg = arg_elem->tk->aspects.front()->tk.get();
    std::string datum_code = qt_arg->aspects[2]->tk->to_code();
    // Place code within application to make sure resulting AST
    //  is an expression
    datum_code = "(" + datum_code + ")";
    //std::cout << datum_code << std::endl;

    // === UNQUOTE ===
    try {
      // Remake PSIL
      auto lang = psil_parser::make_psil_lang();
      // Parse code
      auto ast = psil_parser::parse( lang, datum_code );
      if ( !ast ) { throw 1; } // Error while parsing
      // DEBUG
      //ast->print();
      //std::cout << ast->to_code() << std::endl;
      
      // === Run eval library to check for error ===
      bool e = psil_eval::check_node( ast.get() );
      if ( !e ) { throw 1; } // Error while evaluating
      
      // === Take result and update eAST ===
      //       <program>            <form>               <expression>
      auto tmp = ast->aspects.front()->tk->aspects.front()->tk.get();
      //               <expression>         <application>
      auto ret = std::move( tmp->aspects.front()->tk );
      // Place result into begin statement
      node->aspects.clear();
      node->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( "(" ) );
      node->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( "begin" ) );
      for ( auto itr = ret->aspects.begin(); itr != ret->aspects.end(); ++itr ) {
	if ( (*itr)->elem_type == TE_Type::TOKEN )
	  node->aspects.push_back( std::move( *itr ) );
      }
      node->aspects.push_back( std::make_unique<psil_parser::token_elem_t>( ")" ) );
    } catch ( ... ) {
      throw std::string( "Error while unquoting" );
    }
  }
}
