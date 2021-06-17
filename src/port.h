#pragma once

#include <chrono>
#include <functional>

template <typename Type>
class SinkPort;

template <typename Type>
class SourcePort;

template <typename Type>
class Port {
public:
    using TimePoint = std::chrono::high_resolution_clock::time_point;

    Port() = default;
    Port(Type value)
        : _data{value} {};

    Port(const Port &) = delete;
    Port &operator=(const Port &) = delete;

    Type &data(TimePoint t) {
        return _data;
    }

    const Type &data(const Type &value) {
        return _data = value;
    }

    virtual ~Port() = default;

protected:
    Type _data;
    TimePoint _updateTime = {};
};

//! Port used to produce new data
template <typename Type>
class SourcePort : public Port<Type> {
public:
    using PortT = Port<Type>;

    SourcePort(Type initValue = {})
        : PortT{std::move(initValue)} {}

    const Type &data() {
        update(this->_updateTime);
        return this->_data;
    }

    virtual void update(typename PortT::TimePoint t) = 0;

    auto &port() {
        return _port;
    }

    operator PortT &() {
        return _port;
    }

    PortT _port;
};

template <typename Type>
class SinkPort : public SourcePort<Type> {
public:
    using SourcePortT = SourcePort<Type>;
    using UpdateFunctionT = std::function<void(SinkPort<Type> &)>;

    SinkPort(UpdateFunctionT f, Type initValue = {})
        : SourcePort<Type>{std::move(initValue)}
        , _f{f} {}

    void update(typename SourcePortT::PortT::TimePoint t) override {
        if (t > this->_updateTime) {
            this->_f(*this);
            this->_updateTime = t;
        }
    }

    //! Return reference for writing to port
    auto &data() {
        return this->_data;
    }

private:
    Port<Type> _port;
    UpdateFunctionT _f = {};
};
