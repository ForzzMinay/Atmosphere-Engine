#pragma once
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <atomic>
#include <memory>

#include "../model/Track.hpp"
#include "../dsp/AudioAnalyzer.hpp"
#include "../nlp/TextAnalyzer.hpp"
#include "../storage/LibraryDatabase.hpp"
#include "../storage/VectorIndex.hpp"

namespace atmosphere::pipeline {

struct PipelineConfig {
    size_t num_threads{std::thread::hardware_concurrency() > 0
                           ? std::thread::hardware_concurrency()
                           : 4};
    std::string db_path{"atmosphere.db"};

    // Callback-и — так core остаётся независимым от Qt
    std::function<void(const std::string& file, float progress)> on_progress;
    std::function<void(const model::Track& track)> on_track_complete;
    std::function<void(size_t completed, size_t total)> on_batch_progress;
};

class AnalysisPipeline {
public:
    explicit AnalysisPipeline(PipelineConfig config);
    ~AnalysisPipeline();

    void enqueue(const std::string& file_path);
    void enqueue_batch(const std::vector<std::string>& file_paths);

    void start();
    void stop();
    void wait_until_idle();

    [[nodiscard]] size_t queue_size() const;
    [[nodiscard]] bool is_running() const;

private:
    void worker_thread();

    PipelineConfig config_;
    std::vector<std::jthread> workers_;

    mutable std::mutex queue_mutex_;
    std::condition_variable queue_cv_;
    std::queue<std::string> task_queue_;

    std::atomic<bool> running_{false};
    std::atomic<bool> stop_requested_{false};
    std::atomic<size_t> processed_count_{0};
    std::atomic<size_t> total_count_{0};

    std::unique_ptr<dsp::AudioAnalyzer> dsp_analyzer_;
    std::unique_ptr<nlp::TextAnalyzer> nlp_analyzer_;
    std::unique_ptr<storage::LibraryDatabase> database_;
    std::unique_ptr<index::VectorIndex> vector_index_;
};

} // namespace atmosphere::pipeline