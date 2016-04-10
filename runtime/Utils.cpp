#include <cstdint>
#include <string>
#include <memory>

#include "frontend/AST.hpp"
#include "Value.hpp"

#define TYPE_PAIR(t1,t2) ((static_cast<int>(t1) << 4) | static_cast<int>(t2))

namespace Operations {
    Value add(Value val1, Value val2) {
        switch (TYPE_PAIR(val1.type, val2.type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return Value(getIntValue(val1) + getIntValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return Value(getDoubleValue(val1) + static_cast<double>(getIntValue(val2)));
                break;
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return Value(static_cast<double>(getIntValue(val1)) + getDoubleValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return Value(getDoubleValue(val1) + getDoubleValue(val2));
                break;
            case TYPE_PAIR(ValueType::STRING, ValueType::STRING):
                return Value(getStringValue(val1) + getStringValue(val2));
                break;
            default:
                printf("error: unsupported operand type(s) for +\n");
                exit(1);
        }
    }

    Value sub(Value val1, Value val2) {
        switch (TYPE_PAIR(val1.type, val2.type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return Value(getIntValue(val1) - getIntValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return Value(getDoubleValue(val1) - static_cast<double>(getIntValue(val2)));
                break;
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return Value(static_cast<double>(getIntValue(val1)) - getDoubleValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return Value(getDoubleValue(val1) - getDoubleValue(val2));
                break;
            default:
                printf("error: unsupported operand type(s) for -\n");
                exit(1);
        }
    }

    Value mul(Value val1, Value val2) {
        switch (TYPE_PAIR(val1.type, val2.type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return Value(getIntValue(val1) * getIntValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return Value(getDoubleValue(val1) * static_cast<double>(getIntValue(val2)));
                break;
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return Value(static_cast<double>(getIntValue(val1)) * getDoubleValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return Value(getDoubleValue(val1) * getDoubleValue(val2));
                break;
            case TYPE_PAIR(ValueType::STRING, ValueType::INT):
                {
                    std::string out;
                    std::string in = getStringValue(val1);
                    for (int64_t i = 0; i < getIntValue(val2); ++i) {
                        out += in;
                    }
                    return Value(out);
                }
                break;
            default:
                printf("error: unsupported operand type(s) for *\n");
                exit(1);
        }
    }

    Value div(Value val1, Value val2) {
        int64_t a, b;
        switch (TYPE_PAIR(val1.type, val2.type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                a = getIntValue(val1);
                b = getIntValue(val2);
                if (a % b == 0) {
                    return Value(a / b);
                } else {
                    return Value(static_cast<double>(a) / b);
                }
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return Value(getDoubleValue(val1) / static_cast<double>(getIntValue(val2)));
                break;
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return Value(static_cast<double>(getIntValue(val1)) / getDoubleValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return Value(getDoubleValue(val1) / getDoubleValue(val2));
                break;
            default:
                printf("error: unsupported operand type(s) for /\n");
                exit(1);
        }
    }

    Value lessThan(Value val1, Value val2) {
        switch (TYPE_PAIR(val1.type, val2.type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return Value(getIntValue(val1) < getIntValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return Value(getDoubleValue(val1) < static_cast<double>(getIntValue(val2)));
                break;
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return Value(static_cast<double>(getIntValue(val1)) < getDoubleValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return Value(getDoubleValue(val1) < getDoubleValue(val2));
                break;
            default:
                printf("error: unsupported operand type(s) for <\n");
                exit(1);
        }
    }

    Value greaterThan(Value val1, Value val2) {
        switch (TYPE_PAIR(val1.type, val2.type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return Value(getIntValue(val1) > getIntValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return Value(getDoubleValue(val1) > static_cast<double>(getIntValue(val2)));
                break;
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return Value(static_cast<double>(getIntValue(val1)) > getDoubleValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return Value(getDoubleValue(val1) > getDoubleValue(val2));
                break;
            default:
                printf("error: unsupported operand type(s) for >\n");
                exit(1);
        }
    }

    Value logicalAnd(Value val1, Value val2) {
        return Value(getBoolValue(val1) && getBoolValue(val2));
    }

    Value logicalOr(Value val1, Value val2) {
        return Value(getBoolValue(val1) || getBoolValue(val2));
    }

    Value performBinary(Value value1, BinaryOp op, Value value2) {
        switch (op) {
            case BinaryOp::ADD:
                return Operations::add(value1, value2);
                break;
            case BinaryOp::SUBTRACT:
                return Operations::sub(value1, value2);
                break;
            case BinaryOp::MULTIPLY:
                return Operations::mul(value1, value2);
                break;
            case BinaryOp::DIVIDE:
                return Operations::div(value1, value2);
                break;
            case BinaryOp::GREATER_THAN:
                return Operations::greaterThan(value1, value2);
                break;
            case BinaryOp::LESS_THAN:
                return Operations::lessThan(value1, value2);
                break;
            case BinaryOp::LOGICAL_AND:
                return Operations::logicalAnd(value1, value2);
                break;
            case BinaryOp::LOGICAL_OR:
                return Operations::logicalOr(value1, value2);
                break;
            default:
                printf("error: undefined binary operation, exiting\n");
                exit(1);
        }
    }
};
