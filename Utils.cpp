#include <cstdint>
#include <string>
#include <memory>

#include "Globals.hpp"
#include "AST.hpp"
#include "Value.hpp"


#define TYPE_PAIR(t1,t2) ((static_cast<int>(t1) << 4) | static_cast<int>(t2))

namespace Operations {
    void add(Value &result, Value val1, Value val2) {
        switch (TYPE_PAIR(val1.type, val2.type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                result.set(getIntValue(val1) + getIntValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                result.set(getDoubleValue(val1) + static_cast<double>(getIntValue(val2)));
                break;
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                result.set(static_cast<double>(getIntValue(val1)) + getDoubleValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                result.set(getDoubleValue(val1) + getDoubleValue(val2));
                break;
            case TYPE_PAIR(ValueType::STRING, ValueType::STRING):
                pool.strings.push_back(
                    std::make_pair(
                        1,
                        new String(pool.strings[val1.data.num].second->value + pool.strings[val2.data.num].second->value)
                    )
                );
                result.set(
                    static_cast<int>(pool.strings.size()) - 1,
                    ValueType::STRING
                );
                break;
            default:
                printf("error: unsupported operand type(s) for +\n");
                exit(1);
        }
    }

    void sub(Value &result, Value val1, Value val2) {
        switch (TYPE_PAIR(val1.type, val2.type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                result.set(getIntValue(val1) - getIntValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                result.set(getDoubleValue(val1) - static_cast<double>(getIntValue(val2)));
                break;
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                result.set(static_cast<double>(getIntValue(val1)) - getDoubleValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                result.set(getDoubleValue(val1) - getDoubleValue(val2));
                break;
            default:
                printf("error: unsupported operand type(s) for -\n");
                exit(1);
        }
    }

    void mul(Value &result, Value val1, Value val2) {
        switch (TYPE_PAIR(val1.type, val2.type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                result.set(getIntValue(val1) * getIntValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                result.set(getDoubleValue(val1) * static_cast<double>(getIntValue(val2)));
                break;
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                result.set(static_cast<double>(getIntValue(val1)) * getDoubleValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                result.set(getDoubleValue(val1) * getDoubleValue(val2));
                break;
            case TYPE_PAIR(ValueType::STRING, ValueType::INT):
                {
                    std::string out;
                    std::string in = pool.strings[val1.data.num].second->value;
                    for (int64_t i = 0; i < getIntValue(val2); ++i) {
                        out += in;
                    }
                    pool.strings.push_back (std::make_pair(1, new String(out)));
                    result.set(
                        static_cast<int>(pool.strings.size()) - 1,
                        ValueType::STRING
                    );
                }
                break;
            default:
                printf("error: unsupported operand type(s) for *\n");
                exit(1);
        }
    }

    void div(Value &result, Value val1, Value val2) {
        int64_t a, b;
        switch (TYPE_PAIR(val1.type, val2.type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                a = getIntValue(val1);
                b = getIntValue(val2);
                if (a % b == 0) {
                    result.set(a / b);
                } else {
                    result.set(static_cast<double>(a) / b);
                }
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                result.set(getDoubleValue(val1) / static_cast<double>(getIntValue(val2)));
                break;
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                result.set(static_cast<double>(getIntValue(val1)) / getDoubleValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                result.set(getDoubleValue(val1) / getDoubleValue(val2));
                break;
            default:
                printf("error: unsupported operand type(s) for /\n");
                exit(1);
        }
    }

    void lessThan(Value &result, Value val1, Value val2) {
        switch (TYPE_PAIR(val1.type, val2.type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                result.set(getIntValue(val1) < getIntValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                result.set(getDoubleValue(val1) < static_cast<double>(getIntValue(val2)));
                break;
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                result.set(static_cast<double>(getIntValue(val1)) < getDoubleValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                result.set(getDoubleValue(val1) < getDoubleValue(val2));
                break;
            default:
                printf("error: unsupported operand type(s) for <\n");
                exit(1);
        }
    }

    void greaterThan(Value &result, Value val1, Value val2) {
        switch (TYPE_PAIR(val1.type, val2.type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                result.set(getIntValue(val1) > getIntValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                result.set(getDoubleValue(val1) > static_cast<double>(getIntValue(val2)));
                break;
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                result.set(static_cast<double>(getIntValue(val1)) > getDoubleValue(val2));
                break;
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                result.set(getDoubleValue(val1) > getDoubleValue(val2));
                break;
            default:
                printf("error: unsupported operand type(s) for >\n");
                exit(1);
        }
    }

    void logicalAnd(Value &result, Value val1, Value val2) {
        result.set(getBoolValue(val1) && getBoolValue(val2));
    }

    void logicalOr(Value &result, Value val1, Value val2) {
        result.set(getBoolValue(val1) || getBoolValue(val2));
    }

    void performBinary(Value &result, Value value1, BinaryOp op, Value value2) {
        switch (op) {
            case BinaryOp::ADD:
                Operations::add(result, value1, value2);
                break;
            case BinaryOp::SUBTRACT:
                Operations::sub(result, value1, value2);
                break;
            case BinaryOp::MULTIPLY:
                Operations::mul(result, value1, value2);
                break;
            case BinaryOp::DIVIDE:
                Operations::div(result, value1, value2);
                break;
            case BinaryOp::GREATER_THAN:
                Operations::greaterThan(result, value1, value2);
                break;
            case BinaryOp::LESS_THAN:
                Operations::lessThan(result, value1, value2);
                break;
            case BinaryOp::LOGICAL_AND:
                Operations::logicalAnd(result, value1, value2);
                break;
            case BinaryOp::LOGICAL_OR:
                Operations::logicalOr(result, value1, value2);
                break;
            default:
                printf("error: undefined binary operation, exiting\n");
                exit(1);
        }
    }
};
