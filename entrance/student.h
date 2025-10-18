#pragma once

#include <string>
#include <vector>
#include <chrono>

struct StudentName {
    std::string name;
    std::string surname;

    bool operator==(const StudentName&) const = default;
};

struct StudentInfo {
    StudentName name;
    std::chrono::year_month_day birth_date;
    size_t score;
    std::vector<std::string> university_preference;

    bool operator==(const StudentInfo&) const = default;
};
