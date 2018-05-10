#pragma once

#include <string>
#include <sstream>

#include <node.hpp>
#include <asdffile.hpp>


namespace Asdf {

Node Load(const std::string& input, const AsdfFile* file);
Node Load(const char* input, const AsdfFile* file);
Node Load(std::istream& input, const AsdfFile* file);

} /* namespace Asdf */
