// Notice.h
#pragma once
#include <string>

class Notice {
    std::string title;
    std::string message;

public:
    Notice(std::string t, std::string m) : title(std::move(t)), message(std::move(m)) {}
    const std::string& getTitle() const { return title; }
    const std::string& getMessage() const { return message; }
    void setTitle(const std::string& t) { title = t; }
    void setMessage(const std::string& m) { message = m; }
};
