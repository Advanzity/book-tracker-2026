#pragma once

#include <exception>
#include <string>

class ContainerException : public std::exception
{
private:
    std::string message;

public:
    explicit ContainerException(const std::string &message);

    const char *what() const noexcept override;
};
