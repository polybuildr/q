#include "Value.hpp"

Value::Value(int value): value(value) {

}
void Value::setValue(int val) {
    value = val;
}
int Value:: getValue() {
    return value;
}
