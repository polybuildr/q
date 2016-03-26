#ifndef VALUE_HPP
#define VALUE_HPP

#include "Globals.hpp"
#include <cstdint>
#include <cinttypes>
#include "classes/String.hpp"


enum class ValueType { INT, REAL, CHAR, BOOL, STRING, UNDEFINED };

union ValueData {
    int64_t num;
    double  dbl;
    void *object;
};

struct Value {
    bool constant;
    ValueType type;
    ValueData data;

    Value() {
        type = ValueType::UNDEFINED;
        constant = false;
    }

    Value(const Value &v) {
        type = v.type;
        data = v.data;
        constant = v.constant;
        if (type == ValueType::STRING)
            pool.strings[data.num].first++;
    }

    Value& operator=(const Value& v) {
        type = v.type;
        data = v.data;
        if (type == ValueType::STRING)
            pool.strings[data.num].first++;
        constant = v.constant;
        return *this;
    }

    ~Value() {
        if (type == ValueType::STRING) {
            pool.strings[data.num].first--;
            if (pool.strings[data.num].first == 0) {
                delete pool.strings[data.num].second;
                pool.strings[data.num].first = 0;
                pool.freeStringsList.push_back(data.num);
            }
        }
    }

    Value(int64_t v) {
        constant = false;
        type = ValueType::INT;
        data.num = v;
    }

    void set(int64_t v) {
        constant = false;
        type = ValueType::INT;
        data.num = v;
    }

    Value(double v) {
        constant = false;
        type = ValueType::REAL;
        data.dbl = v;
    }

    void set(double v) {
        constant = false;
        type = ValueType::REAL;
        data.dbl = v;
    }

    Value(bool v) {
        constant = false;
        type = ValueType::BOOL;
        data.num = v;
    }

    void set(bool v) {
        constant = false;
        type = ValueType::BOOL;
        data.num = v;
    }

    Value(int v, ValueType t) {
        constant = false;
        type = ValueType::STRING;
        // TODO: Handle Other Types
        data.num = v;
    }

    void set(int v, ValueType t) {
        constant = false;
        type = ValueType::STRING;
        data.num = v;
    }

    void set(std::string v) {
        String *s = new String(v);
        int idx;
        if (pool.freeStringsList.empty()) {
            pool.strings.push_back(std::make_pair(1, s));
            idx = static_cast<int>(pool.strings.size()) - 1;
        }
        else {
            idx = *(pool.freeStringsList.begin());
            pool.freeStringsList.pop_front();
            pool.strings[idx] = std::make_pair(1, s);
        }
        constant = false;
        type = ValueType::STRING;
        data.num = idx;
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
            case ValueType::STRING:
                printf("%s\n", pool.strings[data.num].second->value.c_str());
                break;
            default:
                printf("error: cannot print, unknown value, exiting\n");
        }
    }
};

int64_t getIntValue(Value v) {
    switch (v.type) {
        case ValueType::INT:
            return v.data.num;
        default:
            printf("error: cannot get int value, exiting\n");
            exit(1);
    }
    return 0;
}

double getDoubleValue(Value v) {
    switch (v.type) {
        case ValueType::INT:
            return v.data.num;
        case ValueType::REAL:
            return v.data.dbl;
        default:
            printf("error: cannot get real value, exiting\n");
            exit(1);
    }
}

bool getBoolValue(Value v) {
    switch (v.type) {
        case ValueType::INT:
        case ValueType::BOOL:
            return v.data.num;
        case ValueType::REAL:
            return v.data.dbl;
        default:
            printf("error: cannot get bool value, exiting\n");
            exit(1);
    }
    return 0;
}

#endif
