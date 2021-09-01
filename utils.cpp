#include "utils.h"

using namespace std;

namespace utils {

string TransformToSpecialChars(const string& text) {

    std::string str;

    for (int i = 0; i < text.size(); ++i) {
        const char current_char = text[i];

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

string ScreenSpecialChars(const std::string& value) {

    string out;

    for (const char current_char : value) {
        switch (current_char) {
        case '\r':
            out.append("\\r"s);
            break;
        case '\n':
            out.append("\\n"s);
            break;
        case '\\':
            out.append("\\\\"s);
            break;
        default:
            out.push_back(current_char);
            break;
        }
    }

    return out;
}

} // namespace utils