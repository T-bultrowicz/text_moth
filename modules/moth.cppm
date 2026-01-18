export module moth;

export import <string>;
export import <iostream>;
import lib;

export {

    class AbstractMoth {
    protected:
        param_t _pos;
        param_t _p_param;
        param_t _vitality;
        bool _active;

        virtual char type() const = 0;
        virtual bool can_consume(char x) const { return x != ' '; }
        virtual param_t move_dist() { return _p_param; }
    public:
        AbstractMoth(param_t pos, param_t p_param, param_t vitality)
          : _pos(pos),
            _p_param(p_param),
            _vitality(vitality),
            _active(true) {}
        virtual ~AbstractMoth() = default;

        void feed_cycle(std::string & text) {
            if(!_active || text.empty())
                return;
            size_t s = text.size(), dist = move_dist();
            size_t move_cost = 10 * dist;

            if (move_cost > _vitality) {
                _active = false;
                _vitality = 0;
            } else {
                _pos = (_pos + dist) % s;
                _vitality -= move_cost;
                if (can_consume(text[_pos])) {
                    if (_vitality + (param_t) text[_pos] > _vitality)
                        _vitality += (param_t) text[_pos];
                    else
                        _vitality = (param_t) -1;
                    text[_pos] = ' ';
                } else if (text[_pos] == ' ') {
                    _vitality = _vitality > 32 ? _vitality - 32 : 0;
                }
            }
        }

        friend std::ostream & operator<<(std::ostream & os, const AbstractMoth & am);
    };

    class BasicMoth : public AbstractMoth {
    private:
        char type() const override { return '*'; }
    public:
        BasicMoth(param_t pos, param_t p_param, param_t vitality)
          : AbstractMoth(pos, p_param, vitality) {}
    };

    class LiteralMoth : public AbstractMoth {
    private:
        char type() const override { return 'A'; }
        bool can_consume(char x) const override {
            return (x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z');
        }
    public:
        LiteralMoth(param_t pos, param_t p_param, param_t vitality)
          : AbstractMoth(pos, p_param, vitality) {} 
    };

    class NumeralMoth : public AbstractMoth {
    private:
        char type() const override { return '1'; }
        bool can_consume(char x) const override {
            return (x >= '0' && x <= '9');
        }
    public:
        NumeralMoth(param_t pos, param_t p_param, param_t vitality)
          : AbstractMoth(pos, p_param, vitality) {} 
    };

    class FussyMoth : public AbstractMoth {
    private:
        param_t _curr_move;
        char type() const override { return '!'; }
        bool can_consume(char x) const override {
            return !((x >= '0' && x <= '9') || 
            (x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z') || x == ' ');
        }
        param_t move_dist() override {
            if (++_curr_move > _p_param)
                _curr_move = 1;
            return _curr_move;
        }
    public:
        FussyMoth(param_t pos, param_t p_param, param_t vitality)
          : AbstractMoth(pos, p_param, vitality), _curr_move(0) {} 
    };

    std::ostream & operator<<(std::ostream & os, const AbstractMoth & am) {
        os << am.type() << ' ' << am._p_param << ' ' 
           << am._pos << ' ' << am._vitality;
        return os;
    }
}