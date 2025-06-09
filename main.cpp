#include "crow_all.h"
#include <fstream>
#include <sstream>
#include <asio.hpp>

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        std::ifstream file("public/index.html");
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    });

    CROW_ROUTE(app, "/api/hello")([]() {
        return "Hello from C++ Crow backend!";
    });

     app.port(9090).multithreaded().run(); 
}
