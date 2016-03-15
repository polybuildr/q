#include <cstdint>
#include <string>
#include <memory>

#include "AST.hpp"
#include "Value.hpp"

#define TYPE_PAIR(t1,t2) ((static_cast<int>(t1) << 4) | static_cast<int>(t2))

namespace Operations {
    std::shared_ptr<Value> add(std::shared_ptr<Value> val1, std::shared_ptr<Value> val2) {
        switch (TYPE_PAIR(val1->type, val2->type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return std::make_shared<Value>(getIntValue(val1) + getIntValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return std::make_shared<Value>(getDoubleValue(val1) + static_cast<double>(getIntValue(val2)));
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return std::make_shared<Value>(static_cast<double>(getIntValue(val1)) + getDoubleValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return std::make_shared<Value>(getDoubleValue(val1) + getDoubleValue(val2));
            default:
                printf("error: unsupported operand type(s) for +\n");
                exit(1);
        }
        return nullptr;
    }

    std::shared_ptr<Value> sub(std::shared_ptr<Value> val1, std::shared_ptr<Value> val2) {
        switch (TYPE_PAIR(val1->type, val2->type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return std::make_shared<Value>(getIntValue(val1) - getIntValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return std::make_shared<Value>(getDoubleValue(val1) - static_cast<double>(getIntValue(val2)));
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return std::make_shared<Value>(static_cast<double>(getIntValue(val1)) - getDoubleValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return std::make_shared<Value>(getDoubleValue(val1) - getDoubleValue(val2));
            default:
                printf("error: unsupported operand type(s) for -\n");
                exit(1);
        }
        return nullptr;
    }

    std::shared_ptr<Value> mul(std::shared_ptr<Value> val1, std::shared_ptr<Value> val2) {
        switch (TYPE_PAIR(val1->type, val2->type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return std::make_shared<Value>(getIntValue(val1) * getIntValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return std::make_shared<Value>(getDoubleValue(val1) * static_cast<double>(getIntValue(val2)));
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return std::make_shared<Value>(static_cast<double>(getIntValue(val1)) * getDoubleValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return std::make_shared<Value>(getDoubleValue(val1) * getDoubleValue(val2));
            default:
                printf("error: unsupported operand type(s) for *\n");
                exit(1);
        }
        return nullptr;
    }

    std::shared_ptr<Value> div(std::shared_ptr<Value> val1, std::shared_ptr<Value> val2) {
        int64_t a, b;
        switch (TYPE_PAIR(val1->type, val2->type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                a = getIntValue(val1);
                b = getIntValue(val2);
                if (a % b == 0) {
                    return std::make_shared<Value>(a / b);
                } else {
                    return std::make_shared<Value>(static_cast<double>(a) / b);
                }
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return std::make_shared<Value>(getDoubleValue(val1) / static_cast<double>(getIntValue(val2)));
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return std::make_shared<Value>(static_cast<double>(getIntValue(val1)) / getDoubleValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return std::make_shared<Value>(getDoubleValue(val1) / getDoubleValue(val2));
            default:
                printf("error: unsupported operand type(s) for /\n");
                exit(1);
        }
        return nullptr;
    }

    std::shared_ptr<Value> lessThan(std::shared_ptr<Value> val1, std::shared_ptr<Value> val2) {
        switch (TYPE_PAIR(val1->type, val2->type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return std::make_shared<Value>(getIntValue(val1) < getIntValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return std::make_shared<Value>(getDoubleValue(val1) < static_cast<double>(getIntValue(val2)));
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return std::make_shared<Value>(static_cast<double>(getIntValue(val1)) < getDoubleValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return std::make_shared<Value>(getDoubleValue(val1) < getDoubleValue(val2));
            default:
                printf("error: unsupported operand type(s) for <\n");
                exit(1);
        }
        return nullptr;
    }

    std::shared_ptr<Value> greaterThan(std::shared_ptr<Value> val1, std::shared_ptr<Value> val2) {
        switch (TYPE_PAIR(val1->type, val2->type)) {
            case TYPE_PAIR(ValueType::INT, ValueType::INT):
                return std::make_shared<Value>(getIntValue(val1) > getIntValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::INT):
                return std::make_shared<Value>(getDoubleValue(val1) > static_cast<double>(getIntValue(val2)));
            case TYPE_PAIR(ValueType::INT, ValueType::REAL):
                return std::make_shared<Value>(static_cast<double>(getIntValue(val1)) > getDoubleValue(val2));
            case TYPE_PAIR(ValueType::REAL, ValueType::REAL):
                return std::make_shared<Value>(getDoubleValue(val1) > getDoubleValue(val2));
            default:
                printf("error: unsupported operand type(s) for >\n");
                exit(1);
        }
        return nullptr;
    }

    std::shared_ptr<Value> logicalAnd(std::shared_ptr<Value> val1, std::shared_ptr<Value> val2) {
        return std::make_shared<Value>(getBoolValue(val1) && getBoolValue(val2));
    }

    std::shared_ptr<Value> logicalOr(std::shared_ptr<Value> val1, std::shared_ptr<Value> val2) {
        return std::make_shared<Value>(getBoolValue(val1) || getBoolValue(val2));
    }

    std::shared_ptr<Value> performBinary(const std::shared_ptr<Value> value1, BinaryOp op, const std::shared_ptr<Value> value2) {
                switch (op) {
                    case BinaryOp::ADD:
                        return Operations::add(value1, value2);
                    case BinaryOp::SUBTRACT:
                        return Operations::sub(value1, value2);
                    case BinaryOp::MULTIPLY:
                        return Operations::mul(value1, value2);
                    case BinaryOp::DIVIDE:
                        return Operations::div(value1, value2);
                    case BinaryOp::GREATER_THAN:
                        return Operations::greaterThan(value1, value2);
                    case BinaryOp::LESS_THAN:
                        return Operations::lessThan(value1, value2);
                    case BinaryOp::LOGICAL_AND:
                        return Operations::logicalAnd(value1, value2);
                    case BinaryOp::LOGICAL_OR:
                        return Operations::logicalOr(value1, value2);
                }
            return nullptr;
    }
};
