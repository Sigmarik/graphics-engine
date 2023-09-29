/**
 * @file gtest.cpp
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief Testing module
 * @version 0.1
 * @date 2023-07-11
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <gtest/gtest.h>

static const char WARNING_TEXT[] = R"""(
WARNING: You are using template tests! Replace them with your own!
These three owls stand here to make sure you
remove the template text before publishing your work.
    A_,,,_A        A_,,,_A        A_,,,_A    
   ((O)V(O))      ((O)V(O))      ((O)V(O))   
  ("\"|"|"/")    ("\"|"|"/")    ("\"|"|"/")  
   \"|"|"|"/      \"|"|"|"/      \"|"|"|"/   
     "| |"          "| |"          "| |"     
      ^ ^            ^ ^            ^ ^      
)""";

TEST(TemplateGroup, TemplateTest) {
    puts(WARNING_TEXT);

    ASSERT_EQ(0, 1);
}
