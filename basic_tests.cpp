#include <iostream>
#include <memory>
#include <cassert> // testing

#include "moth.h"
#include "parser.h"

class Engine {
};

int inputParserTest() {
    std::string x;
    getline(std::cin, x);
    ParsedInput y = Parser::parse_input(x);
    std::cout << (int)y._task << '\n';
    std::cout << y._line_num << ' ';
    for (param_t x : y._params) {
        std::cout << x << ' ';
    }
    std::cout << '\n' << x << '\n';
    return 0;
}

int basicMothTests() {
    std::string food("abc123!$%   ");
    std::vector<std::unique_ptr<AbstractMoth>> moths;
    moths.push_back(std::make_unique<LiteralMoth>(0, 4, 100));
    moths.push_back(std::make_unique<NumeralMoth>(0, 4, 100));
    moths.push_back(std::make_unique<BasicMoth>(0, 4, 100));
    moths.push_back(std::make_unique<FussyMoth>(0, 4, 100));

    for (const auto & a : moths)
        std::cout << *a << '\n';

    for (int i = 0; i < 10; ++i) {
        for (const auto & a : moths)
            a->feed_cycle(food);
        std::cout << "\n after feed cycle nr " << i + 1 << "  ----: " << food << '\n';
        for (const auto & a : moths)
            std::cout << *a << '\n';        
    }
    return 0;
}

// basic tests, to use however one like
int main() {
    inputParserTest();
    // basicMothTests();
    return 0;
}