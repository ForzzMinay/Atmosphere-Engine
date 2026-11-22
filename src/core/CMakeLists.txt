add_library(atmosphere-core STATIC
    model/Track.hpp
    model/VibeVector.hpp
    dsp/AudioAnalyzer.hpp
    nlp/TextAnalyzer.hpp
    storage/LibraryDatabase.hpp
    index/VectorIndex.hpp
    pipeline/AnalysisPipeline.hpp
    pipeline/AnalysisPipeline.cpp
)

target_include_directories(atmosphere-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(kissfft CONFIG REQUIRED)
find_package(SndFile REQUIRED)
find_package(onnxruntime CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED) # или sqlite3, зависит от vcpkg

target_link_libraries(atmosphere-core PUBLIC
    kissfft::kissfft
    SndFile::sndfile
    onnxruntime::onnxruntime
    unofficial::sqlite3::sqlite3
)