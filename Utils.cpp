#include <string>
#include <memory>

#include "Value.hpp"

#define TYPE_PAIR(t1,t2) ((static_cast<int>(t1) << 4) | static_cast<int>(t2))

namespace Operations {
    int getIntValue(std::shared_ptr<Value> value) {
        return std::dynamic_pointer_cast<Integer>(value)->getValue();
    }

    float getFloatValue(std::shared_ptr<Value> value) {
        return std::dynamic_pointer_cast<RealNumber>(value)->getValue();
    }

    bool getBoolValue(std::shared_ptr<Value> value) {
        std::shared_ptr<Boolean> b = std::dynamic_pointer_cast<Boolean>(value);
        if (b) {
            return b->getValue();
        }

        std::shared_ptr<Integer> i = std::dynamic_pointer_cast<Integer>(value);
        if (i) {
            return i->getValue() != 0;
        }

        std::shared_ptr<RealNumber> r = std::dynamic_pointer_cast<RealNumber>(value);
        if (r) {
            return r->getValue() != 0.0;
        }
        printf("error: unexpected fatal error, exiting\n");
        exit(1);
    }

    std::shared_ptr<Value> add(std::shared_ptr<Value> val1, std::shared_ptr<Value> val2) {
        switch (TYPE_PAIR(val1->getType(), val2->getType())) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return std::make_shared<Integer>(getIntValue(val1) + getIntValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return std::make_shared<RealNumber>(getFloatValue(val1) + static_cast<float>(getIntValue(val2)));
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return std::make_shared<RealNumber>(static_cast<float>(getIntValue(val1)) + getFloatValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return std::make_shared<RealNumber>(getFloatValue(val1) + getFloatValue(val2));
            default:
                printf("error: unsupported operand type(s) for +\n");
                exit(1);
        }
        return nullptr;
    }

    std::shared_ptr<Value> sub(std::shared_ptr<Value> val1, std::shared_ptr<Value> val2) {
        switch (TYPE_PAIR(val1->getType(), val2->getType())) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return std::make_shared<Integer>(getIntValue(val1) - getIntValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return std::make_shared<RealNumber>(getFloatValue(val1) - static_cast<float>(getIntValue(val2)));
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return std::make_shared<RealNumber>(static_cast<float>(getIntValue(val1)) - getFloatValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return std::make_shared<RealNumber>(getFloatValue(val1) - getFloatValue(val2));
            default:
                printf("error: unsupported operand type(s) for -\n");
                exit(1);
        }
        return nullptr;
    }

    std::shared_ptr<Value> mul(std::shared_ptr<Value> val1, std::shared_ptr<Value> val2) {
        switch (TYPE_PAIR(val1->getType(), val2->getType())) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return std::make_shared<Integer>(getIntValue(val1) * getIntValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return std::make_shared<RealNumber>(getFloatValue(val1) * static_cast<float>(getIntValue(val2)));
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return std::make_shared<RealNumber>(static_cast<float>(getIntValue(val1)) * getFloatValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return std::make_shared<RealNumber>(getFloatValue(val1) * getFloatValue(val2));
            default:
                printf("error: unsupported operand type(s) for *\n");
                exit(1);
        }
        return nullptr;
    }

    std::shared_ptr<Value> div(std::shared_ptr<Value> val1, std::shared_ptr<Value> val2) {
        int a, b;
        switch (TYPE_PAIR(val1->getType(), val2->getType())) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                a = getIntValue(val1);
                b = getIntValue(val2);
                if (a % b == 0) {
                    return std::make_shared<Integer>(a / b);
                } else {
                    return std::make_shared<RealNumber>(static_cast<float>(a) / b);
                }
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return std::make_shared<RealNumber>(getFloatValue(val1) / static_cast<float>(getIntValue(val2)));
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return std::make_shared<RealNumber>(static_cast<float>(getIntValue(val1)) / getFloatValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return std::make_shared<RealNumber>(getFloatValue(val1) / getFloatValue(val2));
            default:
                printf("error: unsupported operand type(s) for /\n");
                exit(1);
        }
        return nullptr;
    }

    std::shared_ptr<Value> lessThan(std::shared_ptr<Value> val1, std::shared_ptr<Value> val2) {
        switch (TYPE_PAIR(val1->getType(), val2->getType())) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return std::make_shared<Boolean>(getIntValue(val1) < getIntValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return std::make_shared<Boolean>(getFloatValue(val1) < static_cast<float>(getIntValue(val2)));
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return std::make_shared<Boolean>(static_cast<float>(getIntValue(val1)) < getFloatValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return std::make_shared<Boolean>(getFloatValue(val1) < getFloatValue(val2));
            default:
                printf("error: unsupported operand type(s) for <\n");
                exit(1);
        }
        return nullptr;
    }

    std::shared_ptr<Value> greaterThan(std::shared_ptr<Value> val1, std::shared_ptr<Value> val2) {
        switch (TYPE_PAIR(val1->getType(), val2->getType())) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return std::make_shared<Boolean>(getIntValue(val1) > getIntValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return std::make_shared<Boolean>(getFloatValue(val1) > static_cast<float>(getIntValue(val2)));
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return std::make_shared<Boolean>(static_cast<float>(getIntValue(val1)) > getFloatValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return std::make_shared<Boolean>(getFloatValue(val1) > getFloatValue(val2));
            default:
                printf("error: unsupported operand type(s) for >\n");
                exit(1);
        }
        return nullptr;
    }

    std::shared_ptr<Value> logicalAnd(std::shared_ptr<Value> val1, std::shared_ptr<Value> val2) {
        return std::make_shared<Boolean>(getBoolValue(val1) && getBoolValue(val2));
    }

    std::shared_ptr<Value> logicalOr(std::shared_ptr<Value> val1, std::shared_ptr<Value> val2) {
        return std::make_shared<Boolean>(getBoolValue(val1) || getBoolValue(val2));
    }

    std::shared_ptr<Value> performBinary(std::shared_ptr<Value> value1, std::string op, std::shared_ptr<Value> value2) {
            if (op.length() == 1) {
                switch (op[0]) {
                    case '+':
                        return Operations::add(value1, value2);
                    case '-':
                        return Operations::sub(value1, value2);
                    case '*':
                        return Operations::mul(value1, value2);
                    case '/':
                        return Operations::div(value1, value2);
                    case '>':
                        return Operations::greaterThan(value1, value2);
                    case '<':
                        return Operations::lessThan(value1, value2);
                }
            }
            if (op == "&&") {
                return Operations::logicalAnd(value1, value2);
            } else if (op == "||") {
                return Operations::logicalOr(value1, value2);
            }
            return nullptr;
    }
};
