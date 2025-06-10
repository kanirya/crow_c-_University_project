#include "crow_all.h"
#include <vector>
#include <mutex>
#include <fstream>
#include <sstream>
#include "nlohmann/json.hpp"
#include <filesystem>

using json = nlohmann::json;

// Classes for the Campus Management System
class Department {
public:
    std::string id;
    std::string name;
    std::string description;
    std::vector<std::string> course_ids;

    Department() = default;
    
    Department(const std::string& id, const std::string& name, const std::string& description, const std::vector<std::string>& course_ids = {})
        : id(id), name(name), description(description), course_ids(course_ids) {}

    json to_json() const {
        return {
            {"id", id},
            {"name", name},
            {"description", description},
            {"course_ids", course_ids}
        };
    }

    static Department from_json(const json& j) {
        Department d;
        d.id = j["id"].get<std::string>();
        d.name = j["name"].get<std::string>();
        d.description = j["description"].get<std::string>();
        d.course_ids = j["course_ids"].get<std::vector<std::string>>();
        return d;
    }
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

    json to_json() const {
        return {
            {"id", id},
            {"name", name},
            {"department_id", department_id},
            {"credits", credits},
            {"semester", semester},
            {"capacity", capacity},
            {"description", description},
            {"student_ids", student_ids}
        };
    }

    static Course from_json(const json& j) {
        Course c;
        c.id = j["id"].get<std::string>();
        c.name = j["name"].get<std::string>();
        c.department_id = j["department_id"].get<std::string>();
        c.credits = j["credits"].get<int>();
        c.semester = j["semester"].get<int>();
        c.capacity = j["capacity"].get<int>();
        c.description = j["description"].get<std::string>();
        c.student_ids = j["student_ids"].get<std::vector<std::string>>();
        return c;
    }
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

    json to_json() const {
        return {
            {"id", id},
            {"name", name},
            {"age", age},
            {"email", email},
            {"department_id", department_id},
            {"course_ids", course_ids},
            {"address", address}
        };
    }

    static Student from_json(const json& j) {
        Student s;
        s.id = j["id"].get<std::string>();
        s.name = j["name"].get<std::string>();
        s.age = j["age"].get<int>();
        s.email = j["email"].get<std::string>();
        s.department_id = j["department_id"].get<std::string>();
        s.course_ids = j["course_ids"].get<std::vector<std::string>>();
        s.address = j["address"].get<std::string>();
        return s;
    }
};

class Teacher {
public:
    std::string id;
    std::string name;
    std::string email;
    std::string department_id;
    std::vector<std::string> course_ids;

    json to_json() const {
        return {
            {"id", id},
            {"name", name},
            {"email", email},
            {"department_id", department_id},
            {"course_ids", course_ids}
        };
    }

    static Teacher from_json(const json& j) {
        Teacher t;
        t.id = j["id"].get<std::string>();
        t.name = j["name"].get<std::string>();
        t.email = j["email"].get<std::string>();
        t.department_id = j["department_id"].get<std::string>();
        t.course_ids = j["course_ids"].get<std::vector<std::string>>();
        return t;
    }
};

// Global data storage
std::vector<Department> departments;
std::vector<Course> courses;
std::vector<Student> students;
std::vector<Teacher> teachers;
std::mutex data_mutex;

// Helper functions for data persistence
void saveToFile(const std::string& filename) {
    try {
        // Create the data structure
        json data;
        data["departments"] = json::array();
        data["courses"] = json::array();
        data["students"] = json::array();
        data["teachers"] = json::array();

        // Add departments
        for (const auto& dept : departments) {
            data["departments"].push_back(dept.to_json());
        }

        // Add courses
        for (const auto& course : courses) {
            data["courses"].push_back(course.to_json());
        }

        // Add students
        for (const auto& student : students) {
            data["students"].push_back(student.to_json());
        }

        // Add teachers
        for (const auto& teacher : teachers) {
            data["teachers"].push_back(teacher.to_json());
        }

        // Save to file
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file for writing: " << filename << std::endl;
            return;
        }
        file << data.dump(4);  // Pretty print with 4 spaces
        file.close();

        std::cout << "Data saved successfully to " << filename << std::endl;
        std::cout << "Saved " << departments.size() << " departments" << std::endl;
        std::cout << "Saved " << courses.size() << " courses" << std::endl;
        std::cout << "Saved " << students.size() << " students" << std::endl;
        std::cout << "Saved " << teachers.size() << " teachers" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error saving data: " << e.what() << std::endl;
    }
}

void loadFromFile(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "No existing data file found. Using default data." << std::endl;
            return;
        }

        // Parse the JSON data
        json data;
        file >> data;
        file.close();

        // Clear existing data
        departments.clear();
        courses.clear();
        students.clear();
        teachers.clear();

        // Load departments
        if (data.contains("departments")) {
            for (const auto& dept_json : data["departments"]) {
                departments.push_back(Department::from_json(dept_json));
            }
        }

        // Load courses
        if (data.contains("courses")) {
            for (const auto& course_json : data["courses"]) {
                courses.push_back(Course::from_json(course_json));
            }
        }

        // Load students
        if (data.contains("students")) {
            for (const auto& student_json : data["students"]) {
                students.push_back(Student::from_json(student_json));
            }
        }

        // Load teachers
        if (data.contains("teachers")) {
            for (const auto& teacher_json : data["teachers"]) {
                teachers.push_back(Teacher::from_json(teacher_json));
            }
        }

        std::cout << "Data loaded successfully from " << filename << std::endl;
        std::cout << "Loaded " << departments.size() << " departments" << std::endl;
        std::cout << "Loaded " << courses.size() << " courses" << std::endl;
        std::cout << "Loaded " << students.size() << " students" << std::endl;
        std::cout << "Loaded " << teachers.size() << " teachers" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error loading data: " << e.what() << std::endl;
    }
}

int main() {
    // Only initialize demo data if data.json does not exist
    if (!std::filesystem::exists("data.json")) {
        std::cout << "Initializing demo data..." << std::endl;
        departments = {
            Department("CS", "Computer Science", "Computer Science and Engineering Department", {"CS101"}),
            Department("EE", "Electrical Engineering", "Electrical and Electronics Engineering Department", {"EE101"})
        };
        courses = {
            Course{"CS101", "Intro to Programming", "CS", 4, 1, 50, "Learn programming basics.", {"S1"}},
            Course{"EE101", "Circuits 101", "EE", 3, 1, 40, "Introduction to circuits.", {"S2"}}
        };
        students = {
            Student{"S1", "Alice Smith", 20, "alice@example.com", "CS", {"CS101"}, "123 Main St"},
            Student{"S2", "Bob Lee", 21, "bob@example.com", "EE", {"EE101"}, "456 Elm St"}
        };
        teachers = {
            Teacher{"T1", "Dr. John Doe", "john.doe@example.com", "CS", {"CS101"}},
            Teacher{"T2", "Dr. Jane Roe", "jane.roe@example.com", "EE", {"EE101"}}
        };
        saveToFile("data.json");
    }
    loadFromFile("data.json");

    crow::SimpleApp app;

    // Serve static files
    CROW_ROUTE(app, "/")
    ([]() -> crow::response {
        std::ifstream file("public/index.html");
        if (!file.is_open()) {
            return crow::response(404, "Error: Could not open index.html");
        }
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return crow::response(200, content);
    });

    CROW_ROUTE(app, "/add-student")
    ([]() -> crow::response {
        std::ifstream file("public/student_form.html");
        if (!file.is_open()) {
            return crow::response(404, "Error: Could not open student_form.html");
        }
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return crow::response(200, content);
    });

    CROW_ROUTE(app, "/add-course")
    ([]() -> crow::response {
        std::ifstream file("public/course_form.html");
        if (!file.is_open()) {
            return crow::response(404, "Error: Could not open course_form.html");
        }
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return crow::response(200, content);
    });

    CROW_ROUTE(app, "/add-teacher")
    ([]() -> crow::response {
        std::ifstream file("public/teacher_form.html");
        if (!file.is_open()) {
            return crow::response(404, "Error: Could not open teacher_form.html");
        }
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return crow::response(200, content);
    });

    CROW_ROUTE(app, "/view-teachers")
    ([]() -> crow::response {
        std::ifstream file("public/view_teachers.html");
        if (!file.is_open()) {
            return crow::response(404, "Error: Could not open view_teachers.html");
        }
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return crow::response(200, content);
    });

    CROW_ROUTE(app, "/view-courses")
    ([]() -> crow::response {
        std::ifstream file("public/view_courses.html");
        if (!file.is_open()) {
            return crow::response(404, "Error: Could not open view_courses.html");
        }
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return crow::response(200, content);
    });

    CROW_ROUTE(app, "/view-departments")
    ([]() -> crow::response {
        std::ifstream file("public/view_departments.html");
        if (!file.is_open()) {
            return crow::response(404, "Error: Could not open view_departments.html");
        }
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return crow::response(200, content);
    });

    CROW_ROUTE(app, "/view-students")
    ([]() -> crow::response {
        std::ifstream file("public/view_students.html");
        if (!file.is_open()) {
            return crow::response(404, "Error: Could not open view_students.html");
        }
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return crow::response(200, content);
    });

    // API endpoints for departments
    CROW_ROUTE(app, "/api/departments")
    .methods("GET"_method)
    ([]() -> crow::response {
        std::lock_guard<std::mutex> lock(data_mutex);
        json response = json::array();
        for (const auto& dept : departments) {
            response.push_back(dept.to_json());
        }
        return crow::response(200, response.dump());
    });

    // API endpoints for courses
    CROW_ROUTE(app, "/api/courses")
    .methods("GET"_method)
    ([]() -> crow::response {
        std::lock_guard<std::mutex> lock(data_mutex);
        json response = json::array();
        for (const auto& course : courses) {
            response.push_back(course.to_json());
        }
        return crow::response(200, response.dump());
    });

    CROW_ROUTE(app, "/api/departments/<string>/courses")
    .methods("GET"_method)
    ([](const std::string& dept_id) -> crow::response {
        std::lock_guard<std::mutex> lock(data_mutex);
        json response = json::array();
        
        for (const auto& course : courses) {
            if (course.department_id == dept_id) {
                response.push_back(course.to_json());
            }
        }

        return crow::response(200, response.dump());
    });

    CROW_ROUTE(app, "/api/add_course")
    .methods("POST"_method)
    ([](const crow::request& req) -> crow::response {
        try {
            std::cout << "Received course data: " << req.body << std::endl;
            auto data = json::parse(req.body);
            std::cout << "Parsed JSON successfully." << std::endl;
            std::lock_guard<std::mutex> lock(data_mutex);
            std::cout << "Acquired mutex." << std::endl;

            // Validate required fields
            if (!data.contains("id") || !data.contains("name") || !data.contains("department_id") || 
                !data.contains("credits") || !data.contains("semester") || !data.contains("capacity") || 
                !data.contains("description")) {
                std::cerr << "Missing required fields in request" << std::endl;
                return crow::response(400, json{{"success", false}, {"message", "Missing required fields"}}.dump());
            }

            // Check if course ID already exists
            for (const auto& course : courses) {
                if (course.id == data["id"].get<std::string>()) {
                    std::cerr << "Course ID already exists: " << data["id"].get<std::string>() << std::endl;
                    return crow::response(400, json{{"success", false}, {"message", "Course ID already exists"}}.dump());
                }
            }

            // Validate numeric fields
            if (!data["credits"].is_number() || !data["semester"].is_number() || !data["capacity"].is_number()) {
                std::cerr << "Invalid numeric fields" << std::endl;
                return crow::response(400, json{{"success", false}, {"message", "Invalid numeric fields"}}.dump());
            }

            Course course;
            course.id = data["id"].get<std::string>();
            course.name = data["name"].get<std::string>();
            course.department_id = data["department_id"].get<std::string>();
            course.credits = data["credits"].get<int>();
            course.semester = data["semester"].get<int>();
            course.capacity = data["capacity"].get<int>();
            course.description = data["description"].get<std::string>();

            courses.push_back(course);
            saveToFile("data.json");
            std::cout << "Course saved successfully: " << course.id << std::endl;

            return crow::response(200, json{{"success", true}}.dump());
        } catch (const json::parse_error& e) {
            std::cerr << "JSON parse error: " << e.what() << std::endl;
            return crow::response(400, json{{"success", false}, {"message", "Invalid JSON format"}}.dump());
        } catch (const std::exception& e) {
            std::cerr << "Error adding course: " << e.what() << std::endl;
            return crow::response(500, json{{"success", false}, {"message", e.what()}}.dump());
        }
    });

    // API endpoints for students
    CROW_ROUTE(app, "/api/students")
    .methods("GET"_method)
    ([]() -> crow::response {
        std::lock_guard<std::mutex> lock(data_mutex);
        json response = json::array();
        for (const auto& student : students) {
            response.push_back(student.to_json());
        }
        return crow::response(200, response.dump());
    });

    CROW_ROUTE(app, "/api/add_student")
    .methods("POST"_method)
    ([](const crow::request& req) -> crow::response {
        try {
            auto data = json::parse(req.body);
            std::lock_guard<std::mutex> lock(data_mutex);

            Student student;
            student.id = data["id"].get<std::string>();
            student.name = data["name"].get<std::string>();
            student.age = data["age"].get<int>();
            student.email = data["email"].get<std::string>();
            student.department_id = data["department_id"].get<std::string>();
            student.course_ids = data["course_ids"].get<std::vector<std::string>>();
            student.address = data["address"].get<std::string>();

            students.push_back(student);
            saveToFile("data.json");

            return crow::response(200, json{{"success", true}}.dump());
        } catch (const std::exception& e) {
            return crow::response(500, json{{"success", false}, {"message", e.what()}}.dump());
        }
    });

    // API endpoints for teachers
    CROW_ROUTE(app, "/api/teachers")
    .methods("GET"_method)
    ([]() -> crow::response {
        std::lock_guard<std::mutex> lock(data_mutex);
        json response = json::array();
        for (const auto& teacher : teachers) {
            response.push_back(teacher.to_json());
        }
        return crow::response(200, response.dump());
    });

    CROW_ROUTE(app, "/api/add_teacher")
    .methods("POST"_method)
    ([](const crow::request& req) -> crow::response {
        try {
            auto data = json::parse(req.body);
            std::lock_guard<std::mutex> lock(data_mutex);

            Teacher teacher;
            teacher.id = data["id"].get<std::string>();
            teacher.name = data["name"].get<std::string>();
            teacher.email = data["email"].get<std::string>();
            teacher.department_id = data["department_id"].get<std::string>();
            teacher.course_ids = data["course_ids"].get<std::vector<std::string>>();

            teachers.push_back(teacher);
            saveToFile("data.json");

            return crow::response(200, json{{"success", true}}.dump());
        } catch (const std::exception& e) {
            return crow::response(500, json{{"success", false}, {"message", e.what()}}.dump());
        }
    });

    app.port(9090).run();
    return 0;
}
