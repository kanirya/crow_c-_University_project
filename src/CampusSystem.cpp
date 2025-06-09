#include "../include/CampusSystem.h"

void CampusSystem::addStudent(const Student& s) {
    students.push_back(s);
}

void CampusSystem::addFaculty(const Faculty& f) {
    faculties.push_back(f);
}

void CampusSystem::addNotice(const Notice& n) {
    notices.push_back(n);
}

std::vector<Student> CampusSystem::getAllStudents() const {
    return students;
}

std::vector<Faculty> CampusSystem::getAllFaculties() const {
    return faculties;
}

std::vector<Notice> CampusSystem::getAllNotices() const {
    return notices;
}
