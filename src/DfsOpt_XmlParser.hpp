#ifndef DFSOPT_XMLPARSER_HPP
#define DFSOPT_XMLPARSER_HPP

#include <string_view>

namespace DfsOpt {
  struct InputData;

  class XmlParser {
  public: 
    static InputData parse(const std::string_view inFile);
  
    static std::string_view schemaString();
  };
}

#endif
