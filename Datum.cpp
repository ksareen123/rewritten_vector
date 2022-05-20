#include <cmath>
#include <fstream>
#include <sstream>

#include "Datum.h"

// You may want to write this function, but you do not have to
bool isBadDataEntry(const Datum& datum) {
    bool isBad = false;
    if ((datum.compute_positivity()*100 - datum.positivity) > 0.1 || (datum.positivity - datum.compute_positivity()*100 > 0.1)) {
        isBad = true;
    }
    if (datum.total != datum.compute_total()) {
        isBad = true;
    }
    return isBad;
}

std::ostream& operator<<(std::ostream& out, const Datum& datum) {
    out << "Week of: " << datum.week << std::endl;
    out << "------------------------------------------------------" << std::endl;
    out << "Total cases: " << datum.total << std::endl;
    out << "Positive cases: " << datum.positive << std::endl;
    out << "Positivity rate: " << datum.positivity << std::endl;
    // out << "Computed: " << datum.compute_positivity()*100 << std::endl;
    // out << "Diff: " << abs(datum.positivity - datum.compute_positivity()*100) << std::endl;
    // out << "Total Diff: " << abs(datum.total - datum.compute_total()) << std::endl;
}

std::istream& operator>>(std::istream& in, Datum& datum) {
    std::string stringdata;
    getline(in, stringdata);
    std::stringstream linestream(stringdata);
    std::string temp = "";

    getline(linestream, temp, ',');
    datum.week = temp;

    getline(linestream, temp, ',');
    datum.negative = stoi(temp);

    getline(linestream, temp, ',');
    datum.positive = stoi(temp);

    getline(linestream, temp, ',');
    datum.total = stoi(temp);

    getline(linestream, temp, ',');
    datum.positivity = stod(temp);

    getline(linestream, temp);
}

[[nodiscard]] Vector<Datum> readData(std::istream& file) {
    Vector<Datum> data;
    std::string temp;
    getline(file, temp);
    int count = 0;
    while (!file.eof()) {
        Datum unit;
        std::stringstream line;
        std::getline(file, temp);
        if (temp == "") {
            break;
        }
        line << temp;
        line >> unit;
        count++;
        std::cout << count << " ";
        std::cout << unit << std::endl;
        data.push_back(unit);
        
    }
    return data;
}

[[nodiscard]] Vector<Datum> badDataEntries(const Vector<Datum>& data) noexcept {
    Vector<Datum> badData;
    int count = 0;
    for (size_t i = 0; i < data.size(); i++) {
        if (isBadDataEntry(data.at(i))) {
            count++;
            //std::cout << count << ": ";
            //std::cout << data[i];
            badData.push_back(data.at(i));
        }
    }
    return badData;
}

[[nodiscard]] bool goodData(const Vector<Datum>& data) noexcept {
    return (badDataEntries(data).empty());
}