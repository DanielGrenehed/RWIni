//
//  test.cpp
//  RWIni
//
//  Created by daniel grenehed on 4/05/17.
//  Copyright © 2017 daniel grenehed. All rights reserved.
//

#include <string>
#include <iostream>
#include "rwini.hpp"

int main(int argc, char const *argv[]) {

  std::string PATH = argv[0];
  std::size_t last = PATH.find_last_of("/");
  PATH = PATH.substr(0, last);

  RWIni ini;

  bool ini_arg = false;

  for (int i = 1; i < argc; i++) {
    if (ini.init(argv[i]))  {
      if (ini.iniExists())  {
        ini_arg = true;
        break;
      }
    }
  }

  if (!ini_arg) ini.init(PATH+"test.ini");

  int ti;
  if (!ini.getValue_as_Int("test-section", "test-int", ti))
  ini.setValue("test-section", "test-int", "4"); // if cannot get value, set value, int 4, choosen by a fair dice-roll.

  float tf;
  if (!ini.getValue_as_Float("test-section", "test-float", tf))
  ini.setValue("test-section", "test-float", "4.0"); // if cannot get value, set value, int 4, choosen by a fair dice-roll.

  long tl;
  if (!ini.getValue_as_Long("test-section", "test-long", tl))
  ini.setValue("test-section", "test-long", "4"); // if cannot get value, set value, int 4, choosen by a fair dice-roll.

  double td;
  if (!ini.getValue_as_Double("test-section", "test-double", td))
  ini.setValue("test-section", "test-double", "4.0"); // if cannot get value, set value, int 4, choosen by a fair dice-roll.

  bool tb;
  if (!ini.getValue_as_Bool("test-section", "test-bool", tb))
  ini.setValue("test-section", "test-bool", "true"); // if cannot get value, set value

  std::string ts;
  if (!ini.getValue_as_String("test-section", "test-string", ts))
  ini.setValue("test-section", "test-string", "totaly random string written by some sort of human");


  std::cout << "Int:" << ti << " Float:" << tf << " Long:" << tl << " Double:" << td << " Bool:" << tb << " String:" << ts << std::endl;

  return 0;
}
