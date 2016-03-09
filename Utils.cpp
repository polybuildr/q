#include <string>

#include "Value.hpp"

#define TYPE_PAIR(t1,t2) ((static_cast<int>(t1) << 4) | static_cast<int>(t2))

namespace Operations {
    int getIntValue(Value* value) {
        return dynamic_cast<Integer *>(value)->getValue();
    }

    float getFloatValue(Value* value) {
        return dynamic_cast<RealNumber *>(value)->getValue();
    }

    bool getBoolValue(Value *value) {
        return dynamic_cast<Boolean *>(value)->getValue();
    }

    Value* add(Value* val1, Value* val2) {
        switch (TYPE_PAIR(val1->getType(), val2->getType())) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return new Integer(getIntValue(val1) + getIntValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return new RealNumber(getFloatValue(val1) + static_cast<float>(getIntValue(val2)));
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return new RealNumber(static_cast<float>(getIntValue(val1)) + getFloatValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return new RealNumber(getFloatValue(val1) + getFloatValue(val2));
            default:
                printf("error: unsupported operand type(s) for +\n");
                exit(1);
        }
        return nullptr;
    }

    Value* sub(Value* val1, Value* val2) {
        switch (TYPE_PAIR(val1->getType(), val2->getType())) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return new Integer(getIntValue(val1) - getIntValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return new RealNumber(getFloatValue(val1) - static_cast<float>(getIntValue(val2)));
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return new RealNumber(static_cast<float>(getIntValue(val1)) - getFloatValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return new RealNumber(getFloatValue(val1) - getFloatValue(val2));
            default:
                printf("error: unsupported operand type(s) for -\n");
                exit(1);
        }
        return nullptr;
    }

    Value* mul(Value* val1, Value* val2) {
        switch (TYPE_PAIR(val1->getType(), val2->getType())) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return new Integer(getIntValue(val1) * getIntValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return new RealNumber(getFloatValue(val1) * static_cast<float>(getIntValue(val2)));
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return new RealNumber(static_cast<float>(getIntValue(val1)) * getFloatValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return new RealNumber(getFloatValue(val1) * getFloatValue(val2));
            default:
                printf("error: unsupported operand type(s) for *\n");
                exit(1);
        }
        return nullptr;
    }

    Value* div(Value* val1, Value* val2) {
        int a, b;
        switch (TYPE_PAIR(val1->getType(), val2->getType())) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                a = getIntValue(val1);
                b = getIntValue(val2);
                if (a % b == 0) {
                    return new Integer(a / b);
                } else {
                    return new RealNumber(static_cast<float>(a) / b);
                }
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return new RealNumber(getFloatValue(val1) / static_cast<float>(getIntValue(val2)));
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return new RealNumber(static_cast<float>(getIntValue(val1)) / getFloatValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return new RealNumber(getFloatValue(val1) / getFloatValue(val2));
            default:
                printf("error: unsupported operand type(s) for /\n");
                exit(1);
        }
        return nullptr;
    }

    Value* logicalAnd(Value* val1, Value* val2) {
        switch (TYPE_PAIR(val1->getType(), val2->getType())) {
            case TYPE_PAIR(ValueType::BOOL, ValueType::BOOL):
                return new Boolean(getBoolValue(val1) && getBoolValue(val2));
            default:
                printf("error: unsupported operand type(s) for &&\n");
                exit(1);
        }
    }

    Value* logicalOr(Value* val1, Value* val2) {
        switch (TYPE_PAIR(val1->getType(), val2->getType())) {
            case TYPE_PAIR(ValueType::BOOL, ValueType::BOOL):
                return new Boolean(getBoolValue(val1) || getBoolValue(val2));
            default:
                printf("error: unsupported operand type(s) for ||\n");
                exit(1);
        }
    }

    Value* performBinary(Value* value1, std::string op, Value* value2) {
            switch (op[0]) {
                case '+':
                    return Operations::add(value1, value2);
                case '-':
                    return Operations::sub(value1, value2);
                case '*':
                    return Operations::mul(value1, value2);
                case '/':
                    return Operations::div(value1, value2);
            }
            if (op == "&&") {
                return Operations::logicalAnd(value1, value2);
            } else if (op == "||") {
                return Operations::logicalOr(value1, value2);
            }
            return nullptr;
    }
};
