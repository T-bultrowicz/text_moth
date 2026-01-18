export module parser;

export import <string>;
export import <sstream>;
export import <vector>;
export import lib;

export {
    /* Structures representing formatted task given as input. */
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
        Task _task;
        std::vector<param_t> _params; 
    private:
        inline static const size_t PARAMETRES = 4;
        ParsedInput() = default;
        ParsedInput(Task t)
          : _task(t),
            _params(std::vector<param_t>(PARAMETRES, 0)) {}
    };

    /* Class that is able to create ParsedInput objects, by
     * the static method parse_input, which parses given string.
     * ParsedInput contains all necessary info to complete a task, with
     * ONE STANDING OUT RULE!!!:
     *      If Task::TEXT, string given as a parameter will be modified, to
     *      represent text that should be inserted.
     */
    class Parser {
    private:
        inline static const int P_PARAM_MAX = 99;
        
        /* Helper to extract correct task */
        static Task get_task(std::stringstream & ss) {
            std::string str;
            if (!(ss >> str)) {
                return Task::UNRECOGNIZABLE;
            }

            if (str == "TEXT") return Task::TEXT;
            else if (str == "MOTH") return Task::MOTH;
            else if (str == "FEED") return Task::FEED;
            else if (str == "PRINTT") return Task::PRINTT;
            else if (str == "PRINTM") return Task::PRINTM;
            else if (str == "DELETE") return Task::DELETE;
            return Task::UNRECOGNIZABLE;
        }

        /* Helper to extract correct moth type */
        static MothType get_moth(char x) {
            if (x == '*') return MothType::BASIC;
            else if (x == 'A') return MothType::LITERAL;
            else if (x == '1') return MothType::NUMERAL;
            else if (x == '!') return MothType::FUSSY;
            return MothType::UNRECOGNIZABLE;
        }

        static bool wrong_ASCIIsign(char x) {
            return ((int) x < 32) || ((int) x > 126);
        }

        /* Helper to assert that string given is in right format for more
         * advanced parsing.
         */
        static bool check_correct_form(std::string & s) {
            if (s.empty()) return false;
            if (s[0] == ' ' || s[s.size() - 1] == ' ' || wrong_ASCIIsign(s[0]))
                return false;
            
            bool tmp = false;
            for (size_t i = 1; i < s.size(); ++i) {
                if ((tmp && (s[i] == ' ')) || wrong_ASCIIsign(s[i])) {
                    return false;
                }
                tmp = (s[i] == ' ');
            }
            return true;
        }

        /* Extracts int, char, int, int from stringstream. Returning false
         * in case of inproper format. Puts results in param_t form in vector v.
         * Helper for parsing the Task::MOTH.
         */
        static bool extract(std::stringstream & s, std::vector<param_t> & v) {
            char sign;
            for (int i = 0; i < 4; ++i) {
                if (i && s.peek() != ' ')
                    return false;
                if (i == 1) {
                    if (!(s >> sign)) return false;
                    if (!(v[1] = static_cast<size_t>(get_moth(sign))))
                        return false;
                } else {
                    if (!(s >> v[i])) return false;
                }
            }
            return true;
        }

    public:

        /* Main parsing input function. If anything is incorrect about the input, 
         * that can be tested without knowing the queried id of LocalState,
         * returned ParsedInput will be marked as Task::UNRECOGNIZABLE.
         */
        static ParsedInput parse_input(std::string & input) {
            ParsedInput ret(Task::UNRECOGNIZABLE);
            std::stringstream s(input);

            if (!check_correct_form(input))
                return ret;

            ret._task = get_task(s);
            if(s.peek() != ' ') ret._task = Task::UNRECOGNIZABLE;
            if (!(s >> ret._line_num)) ret._task = Task::UNRECOGNIZABLE;

            switch(ret._task) {
                case Task::MOTH: {
                    if(s.peek() != ' ') ret._task = Task::UNRECOGNIZABLE;
                    auto & vec = ret._params;
                    if (!extract(s, vec)) ret._task = Task::UNRECOGNIZABLE;
                    if (!vec[2] || !vec[3] || vec[3] > P_PARAM_MAX)
                        ret._task = Task::UNRECOGNIZABLE;
                    break;
                }
                case Task::FEED: {
                    if(s.peek() != ' ') ret._task = Task::UNRECOGNIZABLE;
                    if (!(s >> ret._params[0])) ret._task = Task::UNRECOGNIZABLE;
                    if (!ret._params[0]) ret._task = Task::UNRECOGNIZABLE;
                    break;
                }
                case Task::TEXT: {
                    if(s.peek() != ' ') ret._task = Task::UNRECOGNIZABLE;
                    if(!(s >> input)) ret._task = Task::UNRECOGNIZABLE;
                    break;
                }
                default: break;
            }
            
            char tmp;
            if (s >> tmp) ret._task = Task::UNRECOGNIZABLE;
            return ret;
        }
    };
}