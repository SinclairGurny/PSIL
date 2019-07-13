/**
   psil.h
   PSIL Basic Common Use Functionality
   @author Sinclair Gurny
   @version 0.5
   July 2019
*/

#pragma once

#include "psil_parser.h"
#include "psil_eval.h"
#include "psil_exec.h"

namespace psil {
  auto make_psil_lang = psil_parser::make_psil_lang;
  auto repl = psil_exec::repl;
  auto run_file = psil_exec::run_file;
}
