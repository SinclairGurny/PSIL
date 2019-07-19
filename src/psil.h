/**
   psil.h
   PSIL Basic Common Use Functionality
   @author Sinclair Gurny
   @version 1.0
   July 2019
*/

#pragma once

#include "psil_parser.h"
#include "psil_eval.h"
#include "psil_exec.h"

namespace psil {
  // Creates PSIL Language
  auto make_psil_lang = psil_parser::make_psil_lang;
  // Runs a line of code
  auto repl = psil_exec::repl;
  // Runs a psil file
  auto run_file = psil_exec::run_file;
}
