#pragma once

#include "student.h"
#include "university.h"

#include <map>
#include <string>
#include <vector>

std::map<std::string, std::vector<StudentName>> GetStudents(
    const std::vector<UniversityInfo>& universities_info,
    std::vector<StudentInfo>&& students_info);
