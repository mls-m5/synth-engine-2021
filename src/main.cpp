
#include "connection.h"
#include "port.h"
#include <cmath>
#include <iostream>
#include <vector>

using ControlSinkPort = SinkPort<float>;
using ControlSourcePort = SourcePort<float>;
using AudioSinkPort = SinkCallbackPort<std::vector<float>>;
using AudioSourcePort = AudioSinkPort::SourcePort;
using AudioPort = Port<std::vector<float>>;

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
        for (auto &i : port.get()) {
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

int main(int argc, char **argv) {
    auto instrument = SineInstrument{};
    auto volume = SinkPort<float>{};

    instrument.ports().volume.connect(&volume);

    volume.set(10);

    auto now = std::chrono::high_resolution_clock::now();

    for (auto v : instrument.ports().audioOut.get(now)) {
        std::cout << v << " ";
    }

    std::cout << std::endl;
}
