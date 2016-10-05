#include <iostream>
#include "TabReader.h"

using namespace std;

int main(int argc, char**argv){
  if (argc == 2)
    Tab t(argv[1]); //so that you can feed a filename in the command line
  else
    Tab t("acdc_hell.gp4"); //highway to hell by default
  return 0;
}
