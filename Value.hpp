#ifndef VALUE_HPP
#define VALUE_HPP

enum class ValueType { INT, REAL, CHAR, BOOL };

class Value {
protected:
    ValueType type;
public:
    virtual void print() = 0;
    ValueType getType();
};

class Integer: public Value {
protected:
    int value;
public:
    Integer(int);
    void print();
    int getValue();
};


class RealNumber: public Value {
protected:
    float value;
public:
    RealNumber(float);
    void print();
    float getValue();
};

class Boolean: public Value {
protected:
    bool value;
public:
    Boolean(bool);
    void print();
    bool getValue();
};

#endif
