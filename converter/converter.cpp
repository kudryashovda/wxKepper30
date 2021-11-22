#include <algorithm>
#include <array>
#include <codecvt>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

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

struct item {
    int id;
    int pid;
    wstring marker;
    int mid;
    wstring name;
    wstring comment;
};

int main() {

    wifstream file("data.txt"s);
    wofstream ofs("db.dat");

    std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
    file.imbue(utf8_locale);
    ofs.imbue(utf8_locale);

    if (!file) {
        return -1;
    }

    vector<item> items;

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

        if (!new_comment.empty() && new_comment[new_comment.size() - 1] == '\r') {
            new_comment.erase(new_comment.size() - 1);
        }

        tokens[5] = new_comment;

        items.push_back({ stoi(tokens[0]), stoi(tokens[1]), tokens[2], stoi(tokens[3]), tokens[4], tokens[5] });
    }

    for (int i = 0; i < items.size(); ++i) {
        if (items[i].marker == L"KDA") {
            items[i].id = items[i].mid;
            items[i].pid = items[items[i].pid].id;
        }
    }

    for (int i = 0; i < items.size(); ++i) {
        if (items[i].marker == L"KDA") {
            ofs << items[i].id << '\t' << items[i].pid << '\t' << items[i].name << '\t' << items[i].comment << '\t' << 'N' << '\n';
        }
    }

    return 0;
}
