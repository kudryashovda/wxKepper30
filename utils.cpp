#include "utils.h"

using namespace std;

namespace utils {

wstring TransformToSpecialChars(const wstring& text) {

    wstring str;

    for (int i = 0; i < text.size(); ++i) {
        const wchar_t current_char = text[i];

        if (current_char == '\\') {
            ++i;
            if (i == text.size()) {
                continue;
            }

            const char escaped_char = text[i];
            switch (escaped_char) {
            case 'n':
                str.push_back('\n');
                break;
            case 't':
                str.push_back('\t');
                break;
            case 'r':
                str.push_back('\r');
                break;
            case '\\':
                str.push_back('\\');
                break;
            default:
                continue;
            }
        } else {
            str.push_back(current_char);
        }
    }

    return str;
}

wstring ScreenSpecialChars(const wstring& value) {

    wstring out;

    for (const wchar_t current_char : value) {
        switch (current_char) {
        case '\t':
            out.append(L"\\t");
            break;
        case '\r':
            out.append(L"\\r");
            break;
        case '\n':
            out.append(L"\\n");
            break;
        case '\\':
            out.append(L"\\\\"s);
            break;
        default:
            out.push_back(current_char);
            break;
        }
    }

    return out;
}

} // namespace utils