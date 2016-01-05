#include <string>

#include "Value.hpp"

namespace Operations {
    int getIntValue(Value* value) {
        return dynamic_cast<Integer *>(value)->getValue();
    }

    bool getBoolValue(Value *value) {
        return dynamic_cast<Boolean *>(value)->getValue();
    }

    Value* performBinary(Value* value1, std::string op, Value* value2) {
        if (value1->getType() == ValueType::INT && value2->getType() == ValueType::INT) {
            int val1 = getIntValue(value1);
            int val2 = getIntValue(value2);
            switch (op[0]) {
                case '+':
                    return new Integer(val1 + val2);
                case '-':
                    return new Integer(val1 - val2);
                case '*':
                    return new Integer(val1 * val2);
                case '/':
                    return new Integer(val1 / val2);
            }
        } else if (value1->getType() == ValueType::BOOL && value2->getType() == ValueType::BOOL) {
            bool val1 = getBoolValue(value1);
            bool val2 = getBoolValue(value2);
            if (op == "&&") {
                return new Boolean(val1 && val2);
            } else if (op == "||") {
                return new Boolean(val1 || val2);
            }
        }
        return nullptr;
    }
};
