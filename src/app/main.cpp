#include <iostream>
#include <string>
#include <vector>

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QJsonDocument>
#include <QJsonObject>

#include "core/pipeline/AnalysisPipeline.hpp"

namespace {

QJsonObject track_to_json(const atmosphere::model::Track& t) {
    QJsonObject obj;
    obj["track"]       = QString::fromStdString(t.file_path);
    obj["duration_ms"] = static_cast<qint64>(t.duration_ms.count());

    QJsonObject vibe;
    vibe["melancholy"]          = t.vibe_vector.melancholy;
    vibe["reflection"]          = t.vibe_vector.reflection;
    vibe["social_subtext"]      = t.vibe_vector.social_subtext;
    vibe["atmosphere_density"]  = t.vibe_vector.atmosphere_density;
    vibe["energy"]              = t.vibe_vector.energy;
    obj["vibe_vector"] = vibe;

    QJsonObject audio;
    audio["bpm"]                    = t.audio_features.bpm;
    audio["spectral_centroid_hz"]   = t.audio_features.spectral_centroid_hz;
    audio["dynamic_range_db"]       = t.audio_features.dynamic_range_db;
    obj["audio_features"] = audio;

    return obj;
}

} // namespace

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Atmosphere Engine");
    QCoreApplication::setApplicationVersion("0.1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription(
        "High-performance C++ engine for music analysis "
        "and discovering underground events.");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption analyzeOption(
        {"a", "analyze"},
        "Analyze tracks in a directory", "dir");
    QCommandLineOption jsonOption(
        {"j", "json"},
        "Output results in JSON format");
    parser.addOption(analyzeOption);
    parser.addOption(jsonOption);
    parser.process(app);

    if (parser.isSet(analyzeOption)) {
        const std::string dir_path =
            parser.value(analyzeOption).toStdString();
        const bool json_mode = parser.isSet(jsonOption);

        std::cout << "🌌 Starting Atmosphere Engine Pipeline for: "
                  << dir_path << "\n";

        atmosphere::pipeline::PipelineConfig config;
        config.num_threads = 4;
        config.db_path     = "atmosphere.db";

        config.on_progress = [](const std::string& file, float progress) {
            std::cout << "\r  [" << int(progress * 100) << "%] "
                      << file << "        " << std::flush;
        };

        config.on_track_complete = [json_mode](const atmosphere::model::Track& t) {
            if (json_mode) {
                QJsonDocument doc(track_to_json(t));
                std::cout << "\n"
                          << doc.toJson(QJsonDocument::Compact).toStdString()
                          << "\n";
            } else {
                std::cout << "\n  ✅ " << t.file_path
                          << " | density=" << t.vibe_vector.atmosphere_density
                          << " energy="    << t.vibe_vector.energy
                          << "\n";
            }
        };

        config.on_batch_progress = [](size_t done, size_t total) {
            std::cout << "\n  📊 Progress: " << done << " / " << total << "\n";
        };

        atmosphere::pipeline::AnalysisPipeline pipeline(config);
        pipeline.start();

        // Эмуляция сканера библиотеки.
        // В реальности здесь LibraryScanner рекурсивно обходит папку
        // и складывает FLAC/MP3/WAV в очередь.
        pipeline.enqueue_batch({
            dir_path + "/3umph_track_01.flac",
            dir_path + "/tewiq_track_02.wav",
            dir_path + "/polaroid_track_03.mp3"
        });

        pipeline.wait_until_idle();
        pipeline.stop();

        std::cout << "\n🎯 Analysis complete. Vector index updated.\n";
        return 0;
    }

    std::cout << "Starting Atmosphere Engine GUI (Qt6)...\n";
    // return app.exec();  // включим, когда появится QML UI
    return 0;
}