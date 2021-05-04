#include "TokenValue.hpp"

TokenValue::TokenValue(string val)
{
    this->val = val;
}

string TokenValue::getValue()
{
    return this->val;
}