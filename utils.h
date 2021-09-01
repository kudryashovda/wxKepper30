#pragma once

#include <sstream>
#include <string>

namespace utils {

std::string TransformToSpecialChars(const std::string& text);
std::string ScreenSpecialChars(const std::string& value);

}