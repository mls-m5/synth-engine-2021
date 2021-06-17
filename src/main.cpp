
#include "port.h"
#include <cmath>
#include <iostream>
#include <vector>

using ControlSinkPort = SinkPort<float>;
using ControlSourcePort = SourcePort<float>;
using AudioSinkPort = SinkPort<std::vector<float>>;
using AudioSourcePort = AudioSinkPort::SourcePort;

class SineInstrument {
public:
    SineInstrument()
        : _audioOut{[this](auto &&port) { update(port); },
                    std::vector<float>(10)} {}

    SineInstrument(const SineInstrument &) = delete;
    SineInstrument &operator=(const SineInstrument &) = delete;

    void update(SinkPort<std::vector<float>> &port) {
        auto phase = 0.;
        for (auto &i : port.data()) {
            i = std::sin(++phase);
        }
    }

    auto audioOut(AudioSourcePort::TimePoint t) -> AudioSourcePort & {
        _audioOut.update(t);
        return _audioOut;
    }

private:
    AudioSinkPort _audioOut;
};

int main(int argc, char **argv) {
    auto instrument = SineInstrument{};

    auto now = std::chrono::high_resolution_clock::now();

    for (auto v : instrument.audioOut(now).data()) {
        std::cout << v << " ";
    }

    std::cout << std::endl;
}
