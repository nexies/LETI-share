//
// Created by nexie on 24.06.2025.
//

// #include <string>
// #include <iostream>
// #include <fstream>

#include "marker_string.h"
#include <fstream>


int main (int argc, char * argv []) {
    std::ifstream fin ("lab1.in");
    if (!fin) {
        std::cerr << "Cannot open input file 'lab1.in'!";
        return 1;
    }
    int n = 0;
    std::string line;
    std::getline(fin, line);
    fin >> n;

    if (!fin) {
        std::cerr << "Error parsing file" << std::endl;
        return 3;
    }

    fin.close();

    marker_string str (line);
    marker_string res = remove_special_symbols_from_word_n(str, n);

    std::cout << "Result: " << res << std::endl;

    std::ofstream fout ("lab1.out");
    if (!fout) {
        std::cerr << "Cannot open output file 'lab1.out'!";
        return 2;
    }
    fout << "Source: " << str << std::endl;
    fout << "Res: " << res << std::endl;
    fout.close();
    return 0;
}