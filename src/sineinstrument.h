#pragma once

#include "audioports.h"
#include "connection.h"
#include <cmath>

class SineInstrument {
public:
    struct Ports {
        AudioSourcePort &audioOut;
        Connection<float> volume = {nullptr, 1};
    };

    SineInstrument()
        : _audioOut{[this](auto &&port, auto t) { update(port, t); },
                    std::vector<float>(10)}
        , _ports{_audioOut} {}

    void update(SinkPort<std::vector<float>> &port,
                AudioSinkPort::TimePoint t) {
        auto phase = 0.;
        auto volume = _ports.volume.get(t);
        for (auto &i : port.data()) {
            i = std::sin(++phase) * volume;
        }
    }

    auto &ports() {
        return _ports;
    }

private:
    AudioSinkPort _audioOut;
    Ports _ports;
};
