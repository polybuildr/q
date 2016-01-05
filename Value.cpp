#include <cstdio>

#include "Value.hpp"

ValueType Value::getType() {
    return type;
}

Integer::Integer(int value): value(value) {
    type = ValueType::INT;
}
void Integer::print() {
    printf("%d\n", value);
}

int Integer::getValue() {
    return value;
}

Boolean::Boolean(bool value): value(value) {
    type = ValueType::BOOL;
}

void Boolean::print() {
    if (value) {
        printf("true\n");
    } else {
        printf("false\n");
    }
}

bool Boolean::getValue() {
    return value;
}
