#include "Transation.hpp"

Transation::Transation(State* from,State* to,string condition){
    this->from = from;
    this->to = to;
    this->condition = condition;
}