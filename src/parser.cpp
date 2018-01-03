#include <yaml-cpp/yaml.h>
#include <private/parser.hpp>
#include "nodebuilder.h"

namespace Asdf {

Node Load(const std::string& input) {
  std::stringstream stream(input);
  return Load(stream);
}

Node Load(const char* input) {
  std::stringstream stream(input);
  return Load(stream);
}

Node Load(std::istream& input) {
  YAML::Parser parser(input);
  NodeBuilder builder;
  if (!parser.HandleNextDocument(builder)) {
    return Node();
  }

  return builder.Root();
}

} /* namespace Asdf */
