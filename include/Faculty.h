#pragma once
#include <string>

class Faculty {
private:
    std::string name;
    std::string facultyId;
    std::string subject;

public:
    Faculty(std::string name, std::string id, std::string subject);
    std::string getName() const;
    std::string getId() const;
    std::string getSubject() const;
};
