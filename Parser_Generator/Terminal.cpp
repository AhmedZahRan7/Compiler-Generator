#include "Terminal.hpp"

Terminal::Terminal(string key) {
    this->key = key;
}

string Terminal::getKey() {return this->key;}
