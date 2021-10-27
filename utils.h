#pragma once

#include <sstream>
#include <string>

namespace utils {

std::wstring TransformToSpecialChars(const std::wstring& text);
std::wstring ScreenSpecialChars(const std::wstring& value);

} // namespace utils