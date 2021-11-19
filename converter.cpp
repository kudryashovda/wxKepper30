#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

int main() {

    ifstream file("../data.txt"s);

    if (!file) {
        return -1;
    }

    vector<array<int, 3>> closure;
    vector<array<string, 2>> name_cmt;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string marker;
        string name;
        string comment;
        array<int, 3> arr;
        ss >> arr[0] >> arr[1] >> marker >> arr[2] >> name >> comment;
        if (marker != "KDA") {
            continue;
        }

        closure.push_back(move(arr));
        name_cmt.push_back({ name, comment });
    }

    for (int i = 0; i < closure.size(); ++i) {
        closure[i][0] = closure[i][2];
        closure[i][1] = closure[closure[i][0]][1];
    }

    ofstream ofs("../db.dat");

    for (int i = 0; i < closure.size(); ++i) {
        ofs << closure[i][0] << '\t' << closure[i][1] << '\t' << name_cmt[i][0] << '\t' << name_cmt[i][1] << '\t' << 'N' << '\n';
    }

    return 0;
}
