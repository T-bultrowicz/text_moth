#ifndef MOTH_H
#define MOTH_H

#include "lib.h"

class AbstractMoth {

    protected:
        param_t _pos;
        param_t _p_param;
        param_t _vitality;
        bool _active;

    public:
        AbstractMoth() = default;
        virtual ~AbstractMoth() = default;

        // added so ostream<< can be implemented in parent class.
        virtual std::string type() const = 0;
        virtual void feed_cycle(std::string & text) const = 0;

        friend std::ostream & operator<<(std:: ostream & os, const AbstractMoth & am);
};

class BasicMoth : AbstractMoth {

};

class LiteralMoth : AbstractMoth {

};

class NumeralMoth : AbstractMoth {

};

class FussyMoth : AbstractMoth {

};

std::ostream & operator<<(std:: ostream & os, const AbstractMoth & am) {
    os << am.type() << ' ' << am._p_param << ' ' 
                            << am._pos << ' ' << am._vitality;
    return os;
}

#endif // MOTH_H