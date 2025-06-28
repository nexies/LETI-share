#include "diamond_engine.h"
#include <list>
#include "nx_list.h"
#include <chrono>


int main(int argc, char * argv [])
{
    std::chrono::high_resolution_clock::time_point tp;
    auto tick = [&tp] (const std::string & title = {}) {
        std::cout << "Starting timer for " << title << "..." <<std::endl;
        tp = std::chrono::high_resolution_clock::now();
    };
    auto tack = [&tp] (const std::string & title = {}) {
        auto dur = std::chrono::high_resolution_clock::now() - tp;
        std::cout << title << " took " << std::chrono::duration_cast<std::chrono::nanoseconds>(dur) << " ns" << std::endl;
    };

    nx::list<int> nx_list;
    std::list<int> std_list;

    tick("filling nx_list");
    for (int i = 0; i < 1000; i++)
        nx_list.push_back(-i);
    tack("filling nx_list");

    tick("filling std_list");
    for (int i = 0; i < 1000; i++)
        std_list.push_back(-i);
    tack("filling std_list");

    tick("sorting nx_list");
    nx_list.sort();
    tack("sorting nx_list");

    tick("sorting std_list");
    std_list.sort();
    tack("sorting std_list");

    auto last = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; ++i) {
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - last) << std::endl;
        last = std::chrono::high_resolution_clock::now();
    }

    // diamond::diamond_engine engine;
    // return engine.exec();




}

