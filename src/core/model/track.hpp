#pragma once
#include <string>
#include <chrono>
#include "VibeVector.hpp"

namespace atmosphere::model {

struct AudioFeatures {
    float bpm{0.0f};
    float spectral_centroid_hz{0.0f};
    float dynamic_range_db{0.0f};
};

struct Track {
    std::string id;
    std::string file_path;
    std::string title;
    std::string artist;
    std::string lyrics;
    std::chrono::milliseconds duration_ms{0};
    
    AudioFeatures audio_features;
    VibeVector vibe_vector;
};

} // namespace atmosphere::model