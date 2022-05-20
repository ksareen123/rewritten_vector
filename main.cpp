#include "Datum.h"
#include "Vector.h"
#include <fstream>
#include <iostream>

int main() {
    // std::ifstream file("../input-files/covid-weekly-fall2021.csv");
    // Vector<Datum> data = readData(file);
    // int count = 0;
    // for (int i = 0; i < data.size(); i++) {
    //     count++;
    //     std::cout << count << ": " << data[i] << std::endl;
    // }
    // count = 0;
    // Vector<Datum> bad = badDataEntries(data);
    // std::cout << "Bad entries: " << std::endl;
    // for (int i = 0; i < bad.size(); i++) {
    //     count++;
    //     std::cout << count << ": " << bad[i] << std::endl;
    // }
    // data.erase(data.begin(), data.begin());
    // std::cout << "erased data elements." << std::endl;
    // count = 0;
    // for (int i = 0; i < data.size(); i++) {
    //     count++;
    //     std::cout << count << ": " << data[i] << std::endl;
    // }
    Vector<int> v;
    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);

    for (int i = 0; i < v.size(); i++) {
        
        std::cout << v[i] << std::endl;
    }

    auto i = v.erase(v.begin(), v.begin() + 4);

    for (int i = 0; i < v.size(); i++) {
        
        std::cout << v[i] << std::endl;
    }
    std::cout << *i << std::endl;


    return 0;
}
