
#include "controlports.h"
#include "sineinstrument.h"
#include <iostream>

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
