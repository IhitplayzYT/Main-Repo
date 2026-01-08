#include <exception>
#include <stdexcept>


class PathError : public std::runtime_error {
public:
    PathError(const std::string &s) : std::runtime_error(s) {}
};



