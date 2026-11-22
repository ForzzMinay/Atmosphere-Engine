#pragma once
#include <string>
#include "../model/VibeVector.hpp"

namespace atmosphere::nlp {

class TextAnalyzer {
public:
    TextAnalyzer() = default;

    [[nodiscard]] model::VibeVector analyze_lyrics(const std::string& lyrics) const {
        // TODO: ONNX Runtime инференс мультиязычной модели
        // (рефлексия, меланхолия, социальный подтекст)
        (void)lyrics;
        return model::VibeVector{
            .melancholy = 0.85f,
            .reflection = 0.92f,
            .social_subtext = 0.40f,
            .atmosphere_density = 0.88f,
            .energy = 0.31f
        };
    }
};

} // namespace atmosphere::nlp