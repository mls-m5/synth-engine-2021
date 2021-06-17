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

    Port(const Port &) = delete;
    Port &operator=(const Port &) = delete;

    //! Update and get internal data
    virtual const Type &get(TimePoint t) = 0;

    //! Update port data
    virtual void update(TimePoint t) = 0;

    virtual ~Port() = default;
};

//! A port that is red from, causing the port to update its data
template <typename Type>
class SourcePort : public Port<Type> {
public:
    using PortT = Port<Type>;

    SourcePort(Type initValue = {})
        : _data{std::move(initValue)} {}

    const Type &get(typename PortT::TimePoint t) override {
        this->update(t);
        return this->_data;
    }

protected:
    Type _data;
    typename PortT::TimePoint _updateTime = {};
};

//! Audio sink port is used internally in objects that wishes to write data
template <typename Type>
class SinkPort : public SourcePort<Type> {
public:
    using SourcePortT = SourcePort<Type>;

    using SourcePortT::SourcePortT;

    void update(typename SourcePort<Type>::TimePoint) {}

    //! Return reference for writing to port
    auto &data() {
        return this->_data;
    }

    //! Set the value of the port
    const Type &set(const Type &value) {
        return this->_data = value;
    }
};

//! Sink port that also calls a specified function when updated
template <typename Type>
class SinkCallbackPort : public SinkPort<Type> {
public:
    using SourcePortT = typename SinkPort<Type>::SourcePortT;

    using UpdateFunctionT =
        std::function<void(SinkPort<Type> &, typename SourcePortT::TimePoint)>;

    SinkCallbackPort(UpdateFunctionT f, Type initValue = {})
        : SinkPort<Type>{std::move(initValue)}
        , _f{f} {}

    void update(typename SourcePortT::PortT::TimePoint t) override {
        if (t > this->_updateTime) {
            this->_updateTime = t;
            this->_f(*this, this->_updateTime);
        }
    }

private:
    UpdateFunctionT _f = {};
};
