#pragma once
#include <string>
#include "../model/Track.hpp"

namespace atmosphere::dsp {

class AudioAnalyzer {
public:
    AudioAnalyzer() = default;

    [[nodiscard]] model::AudioFeatures analyze(const std::string& file_path) const {
        // TODO: реальная реализация через libsndfile + kissfft
        // (FFT, spectral centroid, rolloff, flatness, RMS, BPM)
        (void)file_path;
        return model::AudioFeatures{
            .bpm = 78.4f,
            .spectral_centroid_hz = 1420.7f,
            .dynamic_range_db = 7.2f
        };
    }
};

} // namespace atmosphere::dsp