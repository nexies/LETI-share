//
// Created by nexie on 24.06.2025.
//

#include "nx_forward_list.h"
#include <iostream>
#include "marker_string.h"
#include <fstream>

int main (int argc, char * argv []) {
    std::ifstream fin ("lab2.in");
    if (!fin) {
        std::cerr << "Cannot open file lab2.in for read" << std::endl;
        return 1;
    }

    std::string line;
    // std::getline(fin, line);
    // marker_string source { line };
    // nx::forward_list<marker_string> list = source.split(' ', false);
    nx::forward_list<marker_string> list;

    std::getline(fin, line);
    marker_string sep { line };
    std::getline(fin, line);
    marker_string elem { line };

    while (!fin.eof()) {
        std::getline(fin, line);
        list.push_front(line);
    }

    list.reverse();
    auto source_list = list;


    for (auto it = list.begin(); it != list.end(); ++it) {
        if (*it == sep)
            list.insert(++it, elem);
    }
    std::cout << "Result: " << std::endl;
    for (auto i: list) std::cout << i << std::endl;

    std::ofstream fout ("lab2.out");
    if (!fout) {
        std::cerr << "Cannot open file lab2.out for write" << std::endl;
        return 2;
    }

    fout << "Element: " << sep << std::endl;
    fout << "Inserted: " << elem << std::endl;
    fout << "Source :" << std::endl;
    for (auto it = source_list.begin(); it != source_list.end(); ++it)
        fout << *it << std::endl;
    fout << "Result: " << std::endl;
    for (auto it = list.begin(); it != list.end(); ++it)
        fout << *it << std::endl;

    fout.close();

    return 0;
}