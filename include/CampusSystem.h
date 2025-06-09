// #pragma once
// #include <vector>
// #include <optional>
// #include "Student.h"
// #include <algorithm>

// class CampusSystem {
//     std::vector<Student> students;
//     int nextId = 1;

// public:
//     std::vector<Student> getAllStudents() const { return students; }

//     Student addStudent(const std::string& name, int age) {
//         Student s(nextId++, name, age);
//         students.push_back(s);
//         return s;
//     }

//     bool deleteStudent(int id) {
//         auto it = std::remove_if(students.begin(), students.end(), [id](const Student& s){ return s.getId() == id; });
//         if (it == students.end()) return false;
//         students.erase(it, students.end());
//         return true;
//     }

//     std::optional<Student> getStudent(int id) const {
//         for (const auto& s : students) if (s.getId() == id) return s;
//         return std::nullopt;
//     }

//     bool updateStudent(int id, const std::string& name, int age) {
//         for (auto& s : students) {
//             if (s.getId() == id) {
//                 s.setName(name);
//                 s.setAge(age);
//                 return true;
//             }
//         }
//         return false;
//     }
// };
