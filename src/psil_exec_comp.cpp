/**
   psil_exec_comp.cpp
   PSIL Execution Library
   Global Comparison Function Implementations
   @author Sinclair Gurny
   @version 0.5
   July 2019
*/

#include "psil_exec.h"

namespace psil_exec {

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
  
  
}
