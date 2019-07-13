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
}
