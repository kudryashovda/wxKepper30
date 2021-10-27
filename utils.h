#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>

namespace utils {

std::vector<std::wstring> tokenizer(std::wstring str, const std::wstring& delim);
std::wstring TransformToSpecialChars(const std::wstring& text);
std::wstring ScreenSpecialChars(const std::wstring& value);
bool IsValidFilename(const std::wstring& filename);

} // namespace utils