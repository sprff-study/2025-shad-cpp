#include "entrance.h"
#include "university.h"
#include "student.h"

#include <map>
#include <vector>
#include <string>
#include <chrono>

#include <catch2/catch_test_macros.hpp>

using namespace std::chrono_literals;

TEST_CASE("Example") {
    std::vector<UniversityInfo> university_info{
        {"MSU", 1uz},
        {"HSE", 2uz},
        {"MIPT", 100uz},
    };
    std::vector<StudentInfo> students_info{
        {{"Ivan", "Ivanov"}, 1900y / 1 / 1, 100uz, {"MSU", "HSE"}},
        {{"Petr", "Petrov"}, 1900y / 1 / 2, 90uz, {"MSU", "HSE"}},
        {{"Alexander", "Sidorov"}, 1900y / 1 / 3, 110uz, {"MIPT", "HSE"}},
        {{"Ivan", "Petrov"}, 1900y / 1 / 3, 100uz, {"HSE", "MSU", "MIPT"}},
        {{"Petr", "Ivanov"}, 1900y / 1 / 4, 80uz, {"HSE"}},
    };
    auto result = GetStudents(university_info, std::move(students_info));
    const std::map<std::string, std::vector<StudentName>> expected = {
        {"HSE", {{"Ivan", "Petrov"}, {"Petr", "Petrov"}}},
        {"MIPT", {{"Alexander", "Sidorov"}}},
        {"MSU", {{"Ivan", "Ivanov"}}},
    };

    REQUIRE(result.size() == expected.size());
    for (const auto& [k, v] : result) {
        CHECK(v == expected.at(k));
    }
    for (const auto& [k, v] : expected) {
        CHECK(v == result.at(k));
    }
}

TEST_CASE("Order") {
    std::vector<UniversityInfo> university_info{{"X", 1uz}, {"Y", 2uz}, {"Z", 1uz}};
    std::vector<StudentInfo> students_info{{{"A", "A"}, 2000y / 1 / 2, 100uz, {"X", "Y", "Z"}},
                                           {{"A", "B"}, 2000y / 1 / 1, 100uz, {"X", "Y", "Z"}},
                                           {{"A", "A"}, 2000y / 1 / 1, 100uz, {"X", "Y", "Z"}},
                                           {{"B", "A"}, 2000y / 1 / 1, 100uz, {"X", "Y", "Z"}}};
    auto result = GetStudents(university_info, std::move(students_info));

    const std::map<std::string, std::vector<StudentName>> expected = {
        {"X", {{"A", "A"}}}, {"Y", {{"B", "A"}, {"A", "B"}}}, {"Z", {{"A", "A"}}}};

    REQUIRE(result == expected);
}
