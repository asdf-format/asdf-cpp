#pragma once

#include <string>
#include <sstream>

#include <node.hpp>


namespace Asdf {

Node Load(const std::string& input);
Node Load(const char* input);
Node Load(std::istream& input);

} /* namespace Asdf */
