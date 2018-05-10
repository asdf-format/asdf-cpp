#include <yaml-cpp/yaml.h>
#include <private/parser.hpp>
#include "nodebuilder.h"

namespace Asdf {

Node Load(const std::string& input, const AsdfFile* file) {
  std::stringstream stream(input);
  return Load(stream, file);
}

Node Load(const char* input, const AsdfFile* file) {
  std::stringstream stream(input);
  return Load(stream, file);
}

Node Load(std::istream& input, const AsdfFile* file) {
  YAML::Parser parser(input);
  NodeBuilder builder;
  if (!parser.HandleNextDocument(builder)) {
    return Node();
  }

  return builder.Root(file);
}

} /* namespace Asdf */
