#include <fstream>
#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;

int main(int argc, char *argv[]){
    double tl = atof(argv[1]); int ml = atoi(argv[2]); //262144;
    // cout << "tl/ml: ";
    // cin >> tl >> ml;

    ifstream mapping;
    mapping.open("./tests/mapping");
    string pst, st, tc;
    mapping >> pst >> tc;
    vector <vector<string>> tcs(1, vector<string>(1, tc));

    while (mapping >> st >> tc){
        if (st == pst) tcs.back().push_back(tc);
        else tcs.push_back(vector<string>(1, tc));
        pst = st;
    }
    int points[7] = {0, 10, 15, 25, 25, 25, 20};

    ofstream meta;
    meta.open("./package/meta.json");
    meta << "{\n";
    meta << "\t\"name\": \"[HW2.P4] Dungeons of Sacred Abyss\",\n";
    meta << "\t\"timeLimit\": " << tl << ",\n";
    meta << "\t\"memLimit\": " << ml << ",\n";
    meta << "\t\"testdata\": [\n";
    for (int i=0;i<7;++i){
        meta << "\t\t{\n";
        meta << "\t\t\t\"points\": " << points[i] << ",\n";
        meta << "\t\t\t\"tests\": [";
        for (auto &s:tcs[i]){
            meta << '"' << s << "\"";
            if (s != tcs[i].back()) meta << ", ";
        }
        meta << "]\n";
        meta << "\t\t}";
        if (i < 6) meta << ',';
        meta << '\n';
    }
    meta << "\t]\n";

    meta << "}\n";

    return 0;
}