#include "AnalysisPipeline.hpp"

#include <future>
#include <iostream>

namespace atmosphere::pipeline {

AnalysisPipeline::AnalysisPipeline(PipelineConfig config)
    : config_(std::move(config)) {
    dsp_analyzer_   = std::make_unique<dsp::AudioAnalyzer>();
    nlp_analyzer_   = std::make_unique<nlp::TextAnalyzer>();
    database_       = std::make_unique<storage::LibraryDatabase>(config_.db_path);
    vector_index_   = std::make_unique<index::VectorIndex>();
}

AnalysisPipeline::~AnalysisPipeline() {
    stop();
}

void AnalysisPipeline::enqueue(const std::string& file_path) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        task_queue_.push(file_path);
        total_count_.fetch_add(1, std::memory_order_relaxed);
    }
    queue_cv_.notify_one();
}

void AnalysisPipeline::enqueue_batch(const std::vector<std::string>& file_paths) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        for (const auto& path : file_paths) {
            task_queue_.push(path);
            total_count_.fetch_add(1, std::memory_order_relaxed);
        }
    }
    queue_cv_.notify_all();
}

void AnalysisPipeline::start() {
    if (running_.exchange(true)) return;
    stop_requested_.store(false);

    for (size_t i = 0; i < config_.num_threads; ++i) {
        workers_.emplace_back(&AnalysisPipeline::worker_thread, this);
    }
}

void AnalysisPipeline::stop() {
    stop_requested_.store(true);
    queue_cv_.notify_all();
    running_.store(false);
    workers_.clear(); // std::jthread сам делает join()
}

void AnalysisPipeline::wait_until_idle() {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    queue_cv_.wait(lock, [this] {
        return task_queue_.empty() &&
               processed_count_.load() >= total_count_.load();
    });
}

size_t AnalysisPipeline::queue_size() const {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    return task_queue_.size();
}

bool AnalysisPipeline::is_running() const {
    return running_.load();
}

void AnalysisPipeline::worker_thread() {
    while (true) {
        std::string file_path;
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            queue_cv_.wait(lock, [this] {
                return stop_requested_.load() || !task_queue_.empty();
            });

            if (stop_requested_.load() && task_queue_.empty()) {
                return;
            }

            file_path = std::move(task_queue_.front());
            task_queue_.pop();
        }

        if (config_.on_progress) config_.on_progress(file_path, 0.1f);

        // Параллельный DSP и NLP — как описано в архитектуре README
        auto dsp_future = std::async(std::launch::async,
            [this, &file_path]() { return dsp_analyzer_->analyze(file_path); });

        auto nlp_future = std::async(std::launch::async,
            [this, &file_path]() {
                // В реальности текст берётся из TagLib / БД / API
                return nlp_analyzer_->analyze_lyrics(
                    "underground vibes, melancholy, reflection");
            });

        model::AudioFeatures audio_features = dsp_future.get();
        model::VibeVector    vibe_vector    = nlp_future.get();

        if (config_.on_progress) config_.on_progress(file_path, 0.9f);

        model::Track track;
        track.file_path       = file_path;
        track.audio_features  = audio_features;
        track.vibe_vector     = vibe_vector;
        track.duration_ms     = std::chrono::milliseconds(214000);

        database_->save_track(track);
        vector_index_->add_or_update(track.file_path, track.vibe_vector);

        processed_count_.fetch_add(1, std::memory_order_relaxed);

        if (config_.on_track_complete)  config_.on_track_complete(track);
        if (config_.on_batch_progress) {
            config_.on_batch_progress(processed_count_.load(),
                                      total_count_.load());
        }
    }
}

} // namespace atmosphere::pipeline