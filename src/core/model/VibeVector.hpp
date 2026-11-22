#pragma once
#include <cmath>

namespace atmosphere::model {

struct VibeVector {
    float melancholy{0.0f};
    float reflection{0.0f};
    float social_subtext{0.0f};
    float atmosphere_density{0.0f};
    float energy{0.0f};

    [[nodiscard]] float cosine_similarity(const VibeVector& other) const {
        float dot = melancholy * other.melancholy + reflection * other.reflection +
                    social_subtext * other.social_subtext + atmosphere_density * other.atmosphere_density +
                    energy * other.energy;
        
        float mag1 = std::sqrt(melancholy*melancholy + reflection*reflection + social_subtext*social_subtext + 
                               atmosphere_density*atmosphere_density + energy*energy);
        float mag2 = std::sqrt(other.melancholy*other.melancholy + other.reflection*other.reflection + 
                               other.social_subtext*other.social_subtext + other.atmosphere_density*other.atmosphere_density + 
                               other.energy*other.energy);
        
        return (mag1 == 0.0f || mag2 == 0.0f) ? 0.0f : dot / (mag1 * mag2);
    }
};

} // namespace atmosphere::model