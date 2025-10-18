#pragma once

#include "student.h"
#include "university.h"

#include <map>
#include <string>
#include <vector>
#include <algorithm>

bool CmpName(const StudentName& a, const StudentName& b);
bool Cmp(const StudentInfo& a, const StudentInfo& b);

std::map<std::string, std::vector<StudentName>> GetStudents(
    const std::vector<UniversityInfo>& universities_info,
    std::vector<StudentInfo>&& students_info) {

    std::map<std::string, std::vector<StudentName>> result;
    std::map<std::string, size_t> cap;
    for (auto v : universities_info) {
        result[v.name] = {};
        cap[v.name] = v.capacity;
    }
    std::ranges::sort(students_info, Cmp);

    for (auto s : students_info) {
        for (auto u : s.university_preference) {
            if (cap[u] > 0) {
                result[u].push_back(s.name);
                cap[u]--;
                break;
            }
        }
    }

    for (auto& [u, res] : result) {
        std::ranges::sort(res,CmpName);
    }
    
    return result;
}


bool CmpName( const StudentName& a, const StudentName& b) {  // a < b
    if (a.surname != b.surname) {
        return a.surname < b.surname;
    }

    if (a.name != b.name) {
        return a.name < b.name;
    }

    return false;
}

bool Cmp(const StudentInfo& a, const StudentInfo& b) {  // a < b
    if (a.score != b.score) {
        return a.score > b.score;
    }
    if (a.birth_date != b.birth_date) {
        return a.birth_date < b.birth_date;
    }
    if (a.name != b.name) {
        return CmpName(a.name, b.name);
    }
    return false;
}

