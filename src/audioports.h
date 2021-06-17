#pragma once

#include "port.h"

// A sample is a single point in a audio stream
using SampleType = float;

// A frame is the smallest collection of samples calculated each frame update
using FrameType = std::vector<SampleType>;

using AudioSinkPort = SinkCallbackPort<FrameType>;
using AudioSourcePort = SourcePort<FrameType>;
using AudioPort = Port<std::vector<SampleType>>;
