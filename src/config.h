/**
 * @file config.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Project-wide constants
 * @version 0.1
 * @date 2023-07-05
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef CONFIG_H
#define CONFIG_H

static const char PROGRAM_VERSION[] = "v0.1";
static const char PROGRAM_BUG_ADDRESS[] = "<kudriashov.it@phystech.edu>";

static const char PROGRAM_NAME[] =
    "TEMPLATE PROGRAM NAME! REPLACE ME IN THE CONFIG FILE!";

static const char PROGRAM_DESCRIPTION[] =
    "Build from " __DATE__ " " __TIME__
    "\n"
    R"""(WARNING: THIS IS THE TEMPLATE PROGRAM DESCRIPTION!
REPLACE IT IN THE CONFIG FILE!
These owls were tasked to make sure you remove the
template description before publishing your work!
    A_,,,_A        A_,,,_A        A_,,,_A    
   ((O)V(O))      ((O)V(O))      ((O)V(O))   
  ("\"|"|"/")    ("\"|"|"/")    ("\"|"|"/")  
   \"|"|"|"/      \"|"|"|"/      \"|"|"|"/   
     "| |"          "| |"          "| |"     
      ^ ^            ^ ^            ^ ^      )""";

static const unsigned LOG_THRESHOLD = 0;

static const double CMP_EPS = 1e-5;

#endif
