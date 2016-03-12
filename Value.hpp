#ifndef VALUE_HPP
#define VALUE_HPP

#include <cstdint>
#include <cinttypes>
#include <memory>

enum class ValueType { INT, REAL, CHAR, BOOL };

union ValueData {
    int64_t num;
    double  dbl;
};

struct Value {
    ValueType type;
    ValueData data;

    Value(std::shared_ptr<Value> pv) {
        type = pv->type;
        switch (type) {
            case ValueType::INT:
            case ValueType::BOOL:
                data.num = pv->data.num;
                break;
            case ValueType::REAL:
                data.dbl = pv->data.dbl;
                break;
            default:
                printf("error: could not construct new Value, exiting\n");
                exit(1);
        }
    }

    Value() {
        type = ValueType::INT;
        data.num = 0;
    }

    Value(int v) {
        type = ValueType::INT;
        data.num = v;
    }

     Value(double v) {
         type = ValueType::REAL;
         data.dbl = v;
     }

     Value(bool v) {
         type = ValueType::BOOL;
         data.num = v;
     }

     void print() {
         switch (type) {
             case ValueType::INT:
                printf("%" PRId64 "\n", data.num);
                break;
            case ValueType::REAL:
                printf("%lg\n", data.dbl);
                break;
            case ValueType::BOOL:
                if (data.num) {
                    printf("true\n");
                } else {
                    printf("false\n");
                }
                break;
            default:
                printf("error: cannot print, unknown value, exiting\n");
         }
     }
};

int getIntValue(std::shared_ptr<Value> v) {
    switch (v->type) {
        case ValueType::INT:
            return v->data.num;
        default:
            printf("error: cannot get int value, exiting\n");
            exit(1);
    }
    return 0;
}

double getDoubleValue(std::shared_ptr<Value> v) {
    switch (v->type) {
        case ValueType::INT:
            return v->data.num;
        case ValueType::REAL:
            return v->data.dbl;
        default:
            printf("error: cannot get real value, exiting\n");
            exit(1);
    }
}

bool getBoolValue(std::shared_ptr<Value> v) {
    switch (v->type) {
        case ValueType::INT:
        case ValueType::BOOL:
            return v->data.num;
        case ValueType::REAL:
            return v->data.dbl;
        default:
            printf("error: cannot get bool value, exiting\n");
            exit(1);
    }
    return 0;
}

#endif
