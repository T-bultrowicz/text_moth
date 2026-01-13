#ifndef PARSER_H
#define PARSER_H

#include "moth.h"

#include <string>
#include <sstream>
#include <vector>
#include <cinttypes>

using line_num_t = size_t;
using param_t = size_t;

enum class Task : size_t {
    TEXT = 1,
    MOTH = 2,
    FEED = 3,
    PRINTM = 4,
    PRINTT = 5,
    DELETE = 6,
    UNRECOGNIZABLE = 0
};

class Parser;

struct ParsedInput {
    friend class Parser;

    line_num_t _line_num;
    std::vector<param_t> _params; 
    Task _task;
    private:
        inline static const size_t PARAMETRES = 4;
        ParsedInput() = default;
        ParsedInput(Task t)
          : _task(t),
            _params(std::vector<param_t>(PARAMETRES, 0)) {}
};

class Parser {
    private:
        static Task get_task(std::stringstream & ss) {
            std::string str;
            if (!(ss >> str)) {
                return Task::UNRECOGNIZABLE;
            }

            if (str == "TEXT")
                return Task::TEXT;
            else if (str == "MOTH")
                return Task::MOTH;
            else if (str == "FEED")
                return Task::FEED;
            else if (str == "PRINTT")
                return Task::PRINTT;
            else if (str == "PRINTM")
                return Task::PRINTM;
            else if (str == "DELETE")
                return Task::DELETE;
            return Task::UNRECOGNIZABLE;
        }

        static bool wrong_ASCIIsign(char x) {
            return ((int) x < 32) || ((int) x > 126);
        }

        static MothType correct_moth(char x) {
            if (x == '*')
                return MothType::BASIC;
            else if (x == 'A')
                return MothType::LITERAL;
            else if (x == '1')
                return MothType::NUMERAL;
            else if (x == '!')
                return MothType::FUSSY;
            return MothType::UNRECOGNIZABLE;
        }

    public:
        /* Parses user input given as a string, returning ParsedInput struct. 
         *
         * If Task::TEXT is detected, puts the text into a given input string.
         * On other hand Task::UNRECOGNIZABLE marks wrong form of input. */
        static ParsedInput parse_input(std::string & input) {
            ParsedInput ret(Task::UNRECOGNIZABLE);
            std::stringstream s(input);

            // check for empty input or white sign begin
            if (input.empty())
                return ret;
            if (input[0] == ' ' || wrong_ASCIIsign(input[0]))
                return ret;
            // check for double white spaces and other unsuspected signs
            {
                bool tmp = false;
                for (size_t i = 1; i < input.size(); ++i) {
                    if ((tmp && (input[i] == ' ')) || wrong_ASCIIsign(input[i])) {
                        return ret;
                    }
                    tmp = (input[i] == ' ');
                }
            }

            ret._task = get_task(s);
            if (!(s >> ret._line_num)) {
                ret._task = Task::UNRECOGNIZABLE;
                return ret;
            }
            switch(ret._task) {
                case Task::MOTH: {
                    auto & vec = ret._params;
                    char sign;
                    if (!(s >> vec[0] >> sign >> vec[2] >> vec[3])) {
                        ret._task = Task::UNRECOGNIZABLE;
                    }
                    
                    // check basic params correctness
                    if (!vec[3] || vec[3] > 99)
                        ret._task = Task::UNRECOGNIZABLE;
                    if (!(vec[1] = static_cast<size_t>(correct_moth(sign))))
                        ret._task = Task::UNRECOGNIZABLE;
                    break;
                }
                case Task::FEED: {
                    if (!(s >> ret._params[0]))
                        ret._task = Task::UNRECOGNIZABLE;
                    if (!ret._params[0])
                        ret._task = Task::UNRECOGNIZABLE;
                    break;
                }
                case Task::TEXT: {
                    if(!(s >> input)) {
                        ret._task = Task::UNRECOGNIZABLE;
                    }
                    break;
                }
            }
            // Nothing can be left in a stream
            char tmp;
            if (s >> tmp)
                ret._task = Task::UNRECOGNIZABLE;
            return ret;
        }
};

#endif //PARSER_H