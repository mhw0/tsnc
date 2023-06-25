#include <iostream>
#include <compiler/source.h>

tsnc::compiler::Source::Source(const std::string &path): path(path) {
  std::cout << "Hello" << std::endl;
};
