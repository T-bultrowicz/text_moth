#ifndef LIB_H
#define LIB_H

#include <vector>
using line_num_t = size_t;
using param_t = size_t;
enum class MothType : size_t {
    BASIC = 1,
    LITERAL = 2,
    NUMERAL = 3,
    FUSSY = 4,
    UNRECOGNIZABLE = 0
};

#endif