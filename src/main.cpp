#include "crow_all.h"
#include <vector>
#include <mutex>
#include <fstream>
#include <sstream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

// Classes for the Campus Management System
class Department {
public:
    std::string id;
    std::string name;
    std::string description;
    std::vector<std::string> course_ids;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Department, id, name, description, course_ids)
};

class Course {
public:
    std::string id;
    std::string name;
    std::string department_id;
    int credits;
    int semester;
    int capacity;
    std::string description;
    std::vector<std::string> student_ids;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Course, id, name, department_id, credits, semester, capacity, description, student_ids)
};

class Student {
public:
    std::string id;
    std::string name;
    int age;
    std::string email;
    std::string department_id;
    std::vector<std::string> course_ids;
    std::string address;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Student, id, name, age, email, department_id, course_ids, address)
};

class Teacher {
public:
    std::string id;
    std::string name;
    std::string email;
    std::string department_id;
    std::vector<std::string> course_ids;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(Teacher, id, name, email, department_id, course_ids)
};

// Global data storage
std::vector<Department> departments;
std::vector<Course> courses;
std::vector<Student> students;
std::vector<Teacher> teachers;
std::mutex data_mutex;

// Helper functions for data persistence
void save_data() {
    std::lock_guard<std::mutex> lock(data_mutex);
    
    json data = {
        {"departments", departments},
        {"courses", courses},
        {"students", students},
        {"teachers", teachers}
    };

    std::ofstream file("data.json");
    file << data.dump(4);
}

void load_data() {
    std::lock_guard<std::mutex> lock(data_mutex);
    
    try {
        std::ifstream file("data.json");
        if (file.is_open()) {
            json data;
            file >> data;
            
            departments = data["departments"].get<std::vector<Department>>();
            courses = data["courses"].get<std::vector<Course>>();
            students = data["students"].get<std::vector<Student>>();
            teachers = data["teachers"].get<std::vector<Teacher>>();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading data: " << e.what() << std::endl;
    }
}

std::string read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    // Load existing data
    load_data();

    crow::SimpleApp app;

    // Serve static files
    CROW_ROUTE(app, "/")
    ([]() {
        std::string content = read_file("public/index.html");
        return content.empty() ? crow::response(404) : crow::response(200, content);
    });

    CROW_ROUTE(app, "/add-student")
    ([]() {
        std::string content = read_file("public/student_form.html");
        return content.empty() ? crow::response(404) : crow::response(200, content);
    });

    CROW_ROUTE(app, "/add-course")
    ([]() {
        std::string content = read_file("public/course_form.html");
        return content.empty() ? crow::response(404) : crow::response(200, content);
    });

    // API endpoints for departments
    CROW_ROUTE(app, "/api/departments")
    .methods("GET"_method)
    ([]() {
        std::lock_guard<std::mutex> lock(data_mutex);
        json response = {
            {"departments", departments}
        };
        return crow::response(200, response.dump());
    });

    CROW_ROUTE(app, "/api/add_department")
    .methods("POST"_method)
    ([](const crow::request& req) {
        auto data = json::parse(req.body);
        std::lock_guard<std::mutex> lock(data_mutex);

        Department dept;
        dept.id = data["id"];
        dept.name = data["name"];
        dept.description = data["description"];

        departments.push_back(dept);
        save_data();

        return crow::response(200, json{{"success", true}}.dump());
    });

    // API endpoints for courses
    CROW_ROUTE(app, "/api/courses")
    .methods("GET"_method)
    ([]() {
        std::lock_guard<std::mutex> lock(data_mutex);
        json response = {
            {"courses", courses}
        };
        return crow::response(200, response.dump());
    });

    CROW_ROUTE(app, "/api/departments/<string>/courses")
    .methods("GET"_method)
    ([](const std::string& dept_id) {
        std::lock_guard<std::mutex> lock(data_mutex);
        std::vector<Course> dept_courses;
        
        for (const auto& course : courses) {
            if (course.department_id == dept_id) {
                dept_courses.push_back(course);
            }
        }

        json response = {
            {"courses", dept_courses}
        };
        return crow::response(200, response.dump());
    });

    CROW_ROUTE(app, "/api/add_course")
    .methods("POST"_method)
    ([](const crow::request& req) {
        auto data = json::parse(req.body);
        std::lock_guard<std::mutex> lock(data_mutex);

        Course course;
        course.id = data["id"];
        course.name = data["name"];
        course.department_id = data["department_id"];
        course.credits = data["credits"];
        course.semester = data["semester"];
        course.capacity = data["capacity"];
        course.description = data["description"];

        courses.push_back(course);
        save_data();

        return crow::response(200, json{{"success", true}}.dump());
    });

    // API endpoints for students
    CROW_ROUTE(app, "/api/students")
    .methods("GET"_method)
    ([]() {
        std::lock_guard<std::mutex> lock(data_mutex);
        json response = {
            {"students", students}
        };
        return crow::response(200, response.dump());
    });

    CROW_ROUTE(app, "/api/add_student")
    .methods("POST"_method)
    ([](const crow::request& req) {
        auto data = json::parse(req.body);
        std::lock_guard<std::mutex> lock(data_mutex);

        Student student;
        student.id = data["id"];
        student.name = data["name"];
        student.age = data["age"];
        student.email = data["email"];
        student.department_id = data["department_id"];
        student.course_ids = data["course_ids"].get<std::vector<std::string>>();
        student.address = data["address"];

        students.push_back(student);
        save_data();

        return crow::response(200, json{{"success", true}}.dump());
    });

    // API endpoints for teachers
    CROW_ROUTE(app, "/api/teachers")
    .methods("GET"_method)
    ([]() {
        std::lock_guard<std::mutex> lock(data_mutex);
        json response = {
            {"teachers", teachers}
        };
        return crow::response(200, response.dump());
    });

    CROW_ROUTE(app, "/api/add_teacher")
    .methods("POST"_method)
    ([](const crow::request& req) {
        auto data = json::parse(req.body);
        std::lock_guard<std::mutex> lock(data_mutex);

        Teacher teacher;
        teacher.id = data["id"];
        teacher.name = data["name"];
        teacher.email = data["email"];
        teacher.department_id = data["department_id"];
        teacher.course_ids = data["course_ids"].get<std::vector<std::string>>();

        teachers.push_back(teacher);
        save_data();

        return crow::response(200, json{{"success", true}}.dump());
    });

    app.port(8080).multithreaded().run();
    return 0;
} 