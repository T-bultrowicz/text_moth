#include <bits/stdc++.h>

#include "moth.h"
#include "parser.h"

class Engine {
};

// basic test
int main() {
    std::string x;
    getline(std::cin, x);
    ParsedInput y = Parser::parse_input(x);
    std::cout << (int)y._task << '\n';
    std::cout << y._line_num << ' ';
    for (param_t x : y._params) {
        std::cout << x << ' ';
    }
    std::cout << '\n' << x;
}