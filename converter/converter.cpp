#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <codecvt>

using namespace std;

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

int main() {

    wifstream file("data.txt"s);
    wofstream ofs("db.dat");

    std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
    file.imbue(utf8_locale);
    ofs.imbue(utf8_locale);

    if (!file) {
        return -1;
    }

    wstring line;
    while (getline(file, line)) {
        auto tokens = tokenizer(line, L"\t");
        auto& comment = tokens[5];

        wstring new_comment;
        bool end_flag = false;
        for (int i = 0; i + 1 < comment.size(); ++i) {
            if (comment[i] == '/' && comment[i + 1] == 'n') {
                new_comment.push_back('\\');
                new_comment.push_back('n');
                ++i;
                end_flag = true;
            } else {
                new_comment.push_back(comment[i]);
                end_flag = false;
            }
        }

        if (!comment.empty() && !end_flag) {
            new_comment.push_back(comment.back());
        }

        ofs << tokens[0] << '\t' << tokens[1] << '\t' << tokens[4] << '\t' << new_comment << '\t' << 'N' << '\n';
    }

    return 0;
}
