#include "crow_all.h"
#include <vector>
#include <mutex>

struct Student {
    int id;
    std::string name;
    int age;
    std::string course;
};

std::vector<Student> students;
std::mutex mtx;

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([] {
        std::ifstream file("public/index.html");
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    });

    CROW_ROUTE(app, "/api/add_student").methods("POST"_method)([](const crow::request& req) {
        auto json = crow::json::load(req.body);
        if (!json)
            return crow::response(400, "Invalid JSON");

        Student s;
        s.id = json["id"].i();
        s.name = json["name"].s();
        s.age = json["age"].i();
        s.course = json["course"].s();

        {
            std::lock_guard<std::mutex> lock(mtx);
            students.push_back(s);
        }

        return crow::response(200);
    });

    CROW_ROUTE(app, "/api/get_students")([] {
        crow::json::wvalue result;
        {
            std::lock_guard<std::mutex> lock(mtx);

            crow::json::wvalue::list studentList;
            for (const auto& s : students) {
                crow::json::wvalue student_json;
                student_json["id"] = s.id;
                student_json["name"] = s.name;
                student_json["age"] = s.age;
                student_json["course"] = s.course;
                studentList.emplace_back(std::move(student_json));
            }

            result["students"] = std::move(studentList);
        }
        return crow::response(result);
    });

    app.port(9090).multithreaded().run();
}
