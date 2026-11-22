#pragma once
#include <string>
#include "../model/Track.hpp"

namespace atmosphere::storage {

class LibraryDatabase {
public:
    explicit LibraryDatabase(const std::string& db_path) {
        // TODO: sqlite3_open + создание таблиц tracks / vibe_vectors
        (void)db_path;
    }

    bool save_track(const model::Track& track) {
        (void)track;
        return true;
    }

    bool update_vibe_vector(const std::string& file_path, const model::VibeVector& vibe) {
        (void)file_path;
        (void)vibe;
        return true;
    }
};

} // namespace atmosphere::storage