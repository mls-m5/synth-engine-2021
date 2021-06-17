#pragma once

#include "port.h"

template <typename Type>
class Connection : public SourcePort<Type> {
public:
    Connection(Port<Type> *port = {}, Type initValue = {})
        : _port{port}
        , SourcePort<Type>{initValue} {}

    const Type &get(typename SourcePort<Type>::TimePoint t) override {
        if (_port) {
            return _port->get(t);
        }
        else {
            return SourcePort<Type>::get(t);
        }
    }

    void update(typename SourcePort<Type>::TimePoint t) override {
        if (_port) {
            _port->update(t);
        }
    }

    void connect(Port<Type> *port) {
        _port = port;
    }

private:
    Port<Type> *_port;
};
