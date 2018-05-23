#include <yaml-cpp/yaml.h>
#include <asdf-cpp/private/parser.hpp>
#include <asdf-cpp/private/nodebuilder.hpp>

namespace Asdf {

Node Load(const std::string& input, AsdfFile* file) {
  std::stringstream stream(input);
  return Load(stream, file);
}

Node Load(const char* input, AsdfFile* file) {
  std::stringstream stream(input);
  return Load(stream, file);
}

Node Load(std::istream& input, AsdfFile* file) {
  YAML::Parser parser(input);
  NodeBuilder builder;
  if (!parser.HandleNextDocument(builder)) {
    return Node();
  }

  return builder.Root(file);
}

} /* namespace Asdf */
