export module engine;

import <map>;
import <memory>;
import moth;
import parser;

class LocalState {
    private:
        std::string _txt;
        std::vector<std::unique_ptr<AbstractMoth>> _moths;
        void printt() {
            std::cout << _txt << '\n';
        }
        void printm() {
            for (const auto & a : _moths) {
                std::cout << *a << '\n';
            } 
        }

    public:
        LocalState() = default;
        LocalState(std::string_view txt): _txt(txt) {}
        bool update_state(ParsedInput & pi) {
            switch(pi._task) {
                case Task::FEED:
                    for (param_t i = 0; i < pi._params[0]; ++i) {
                        for (const auto & a : _moths) {
                            a->feed_cycle(_txt);
                        }
                    }
                    break;
                case Task::PRINTT:
                    printt(); break;
                case Task::PRINTM:
                    printm(); break;
                case Task::MOTH:
                    if (pi._params[0] >= _txt.size()) {
                        return false;
                    }
                    switch(pi._params[1]) {
                        case static_cast<size_t>(MothType::BASIC):
                            _moths.push_back(std::make_unique<BasicMoth>(
                                pi._params[0], pi._params[3], pi._params[2]));
                                break;
                        case static_cast<size_t>(MothType::LITERAL):
                            _moths.push_back(std::make_unique<LiteralMoth>(
                                pi._params[0], pi._params[3], pi._params[2]));
                                break;
                        case static_cast<size_t>(MothType::NUMERAL):
                            _moths.push_back(std::make_unique<NumeralMoth>(
                                pi._params[0], pi._params[3], pi._params[2]));
                                break;
                        case static_cast<size_t>(MothType::FUSSY):
                            _moths.push_back(std::make_unique<FussyMoth>(
                                pi._params[0], pi._params[3], pi._params[2]));
                                break;
                        default: break;
                    }
                    break;
                default:
                    return false;
            }
            return true;
        }
};


export class GlobalState {
    private:
        using global_state_t = std::map<size_t, LocalState>;
        global_state_t _state{};
        size_t _line_num = 0;
        void inform_error() { std::cerr << "ERROR " << _line_num << '\n'; }

    public: 
        GlobalState();
        ~GlobalState();

        /* Processes input, returns false only if std::cin.eof() is set.
         * Otherwise checks for any input error's and processes correct inputs.
         */
        bool process_input() {
            std::string input;
            ++_line_num;

            if (!getline(std::cin, input)) {
                if (std::cin.eof()) return false;
                inform_error(); return true;
            }
            if (std::cin.eof()) {
                return false;
            }

            // here we parse the input
            ParsedInput pi = Parser::parse_input(input);
            if (pi._task == Task::UNRECOGNIZABLE) {
                inform_error(); return true;
            }
            // inserting new text
            if (pi._task == Task::TEXT) {
                if (_state.find(pi._line_num) != _state.end()) {
                    inform_error(); return true;
                }
                _state.insert({pi._line_num, LocalState(input)});
            } 
            else {
                auto it = _state.find(pi._line_num);
                if (it == _state.end()) {
                    inform_error(); return true;
                }

                // deleting existing text
                if (pi._task == Task::DELETE)
                    _state.erase(it);
                // updating local state in some way 
                else if (!it->second.update_state(pi)) {
                    inform_error();
                }
            }
            return true;
        }
};

GlobalState::GlobalState() = default;
GlobalState::~GlobalState() = default;
