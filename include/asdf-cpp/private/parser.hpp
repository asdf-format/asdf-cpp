#pragma once

#include <string>
#include <sstream>

#include "../node.hpp"
#include "../asdffile.hpp"


namespace Asdf {

Node Load(const std::string& input, AsdfFile* file);
Node Load(const char* input, AsdfFile* file);
Node Load(std::istream& input, AsdfFile* file);

} /* namespace Asdf */
