#include "utils.h"

using namespace std;

namespace utils {

vector<wstring> tokenizer(wstring str, const wstring& delim) {
    vector<wstring> vs;

    size_t pos;
    wstring token;
    do {
        pos = str.find_first_of(delim);
        token = str.substr(0, pos);
        vs.push_back(token);
        str = str.substr(pos + 1);
    } while (pos != wstring::npos);

    return vs;
}

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

bool IsValidFilename(const std::wstring& filename) {

    const unordered_set<wstring> illegal_names = { L"NUL", L"CON", L"PRN", L"AUX", L"COM0", L"COM1", L"COM2", L"COM3", L"COM4", L"COM5", L"COM6", L"COM7", L"COM8", L"COM9", L"LPT0", L"LPT1", L"LPT2", L"LPT3", L"LPT4", L"LPT5", L"LPT6", L"LPT7", L"LPT8", L"LPT9" };
    if (illegal_names.find(filename) != illegal_names.end()) {
        return false;
    }

    const wstring illegal_chars = L"\\/:?\"<>|$";

    for (wchar_t ch : filename) {
        if (illegal_chars.find(ch) != wstring::npos) {
            return false;
        }
    }

    return true;
}

} // namespace utils