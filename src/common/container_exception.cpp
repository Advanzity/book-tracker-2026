#include "common/container_exception.h"

ContainerException::ContainerException(const std::string &message)
    : message(message) {}

const char *ContainerException::what() const noexcept
{
    return message.c_str();
}
