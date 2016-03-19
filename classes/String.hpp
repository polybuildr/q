#include <string>

struct String {
    std::string value;

    String() {}

    String(const char *s) {
        value = s;
    }

    String(std::string s) {
        value = s;
    }

    size_t length() {
        return value.length();
    }
};
