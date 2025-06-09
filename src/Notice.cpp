#include "../include/Notice.h"

Notice::Notice(const std::string& t, const std::string& m) : title(t), message(m) {}

std::string Notice::getTitle() const {
    return title;
}

std::string Notice::getMessage() const {
    return message;
}
