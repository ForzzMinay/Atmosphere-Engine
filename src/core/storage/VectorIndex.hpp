#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <mutex>
#include "../model/VibeVector.hpp"

namespace atmosphere::index {

struct IndexedTrack {
    std::string file_path;
    model::VibeVector vibe;
};

class VectorIndex {
public:
    VectorIndex() = default;

    void add_or_update(const std::string& file_path, const model::VibeVector& vibe) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = std::find_if(index_.begin(), index_.end(),
            [&](const IndexedTrack& t) { return t.file_path == file_path; });
        if (it != index_.end()) {
            it->vibe = vibe;
        } else {
            index_.push_back({file_path, vibe});
        }
    }

    [[nodiscard]] std::vector<std::pair<std::string, float>> search(
        const model::VibeVector& query,
        size_t top_k = 50
    ) const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<std::pair<std::string, float>> scored;
        scored.reserve(index_.size());
        for (const auto& t : index_) {
            scored.emplace_back(t.file_path, t.vibe.cosine_similarity(query));
        }
        std::sort(scored.begin(), scored.end(),
            [](const auto& a, const auto& b) { return a.second > b.second; });
        if (scored.size() > top_k) scored.resize(top_k);
        return scored;
    }

private:
    mutable std::mutex mutex_;
    std::vector<IndexedTrack> index_;
};

} // namespace atmosphere::index