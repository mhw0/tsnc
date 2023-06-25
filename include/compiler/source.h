#pragma once
#include <string>

namespace tsnc {
  namespace compiler {
    class Source {
      public:
        const std::string path;

        Source(const std::string &path);
    };
  }
}
