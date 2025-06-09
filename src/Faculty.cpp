#include "../include/Faculty.h"

Faculty::Faculty(std::string name, std::string id, std::string subject)
    : name(name), facultyId(id), subject(subject) {}

std::string Faculty::getName() const { return name; }
std::string Faculty::getId() const { return facultyId; }
std::string Faculty::getSubject() const { return subject; }
