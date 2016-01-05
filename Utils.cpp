#include <string>

#include "Value.hpp"

namespace Operations {
    int getIntValue(Value* value) {
        return dynamic_cast<Integer *>(value)->getValue();
    }

    static Value* perform(Value* value1, std::string op, Value* value2) {
        if (value1->getType() == ValueType::INT && value2->getType() == ValueType::INT) {
            int int1 = getIntValue(value1);
            int int2 = getIntValue(value2);
            switch (op[0]) {
                case '+':
                    return new Integer(int1 + int2);
                case '-':
                    return new Integer(int1 - int2);
                case '*':
                    return new Integer(int1 * int2);
                case '/':
                    return new Integer(int1 / int2);
            }
        }
        return nullptr;
    }
};
