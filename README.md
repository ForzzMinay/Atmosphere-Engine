# 🌌 Atmosphere Engine

[![Build Status](https://img.shields.io/github/actions/workflow/status/yourusername/atmosphere-engine/ci.yml?branch=main&logo=github)](https://github.com/yourusername/atmosphere-engine/actions)
[![C++](https://img.shields.io/badge/C++-20-blue?logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![Qt](https://img.shields.io/badge/Qt-6.5+-41CD52?logo=qt&logoColor=white)](https://www.qt.io/)
[![QML](https://img.shields.io/badge/QML-Modern-6C6C6C)](https://doc.qt.io/qt-6/qtqml-index.html)
[![CMake](https://img.shields.io/badge/CMake-3.20+-064F8C?logo=cmake&logoColor=white)](https://cmake.org/)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

**Atmosphere Engine** — это десктопное приложение на C++/Qt, которое выходит за рамки стандартных музыкальных сервисов. Движок анализирует треки по «глубине» текстов, эмоциональному окрасу и аудио-характеристикам, помогая находить музыку с плотной атмосферой и смысловой нагрузкой (в духе 3umph, tewiq, Поларойд и андеграунд-сцены).

Всё работает **локально на твоём устройстве** — никаких облаков, полная приватность твоих данных и плейлистов.

## ✨ Ключевые возможности

- 🧠 **Локальный NLP-анализ:** Оценка смысловой нагрузки, рефлексии, меланхолии и социального подтекста через ONNX Runtime и локальные ML-модели. Всё оффлайн.
- 🎧 **Аудио-профилирование:** Интеграция со Spotify Web API для извлечения audio features (valence, energy, acousticness, danceability).
- 📡 **Underground Radar:** Парсинг афиш небольших площадок и нишевых Telegram-каналов для отслеживания первых концертов и андеграунд-ивентов.
- 🎯 **Vibe-Matching:** Поиск треков и событий по кастомным векторам «вайба» (например, `melancholy: 0.8, deep_lyrics: 0.9, energy: 0.3`).
- 🌑 **Атмосферный UI:** Темный минималистичный интерфейс на QML, который настраивается под настроение дня.
- ⚡ **Высокая производительность:** Многопоточный анализ на C++, никаких лагов даже на больших библиотеках.

## 🏗 Архитектура

Приложение построено по принципам Clean Architecture и разделено на независимые слои.

```mermaid
graph TD
    A[QML UI Layer] -->|Qt Signals/Slots| B[C++ Backend]
    B --> C{Vibe Engine}
    C -->|Audio Features| D[Spotify API - cpr]
    C -->|Lyrics NLP| E[ONNX Runtime / llama.cpp]
    C -->|Events| F[Underground Radar Parser]
    B --> G[(SQLite - локальная БД)]
    B --> H[Qt Concurrent Thread Pool]
    C -->|Cache| I[LRU Cache in-memory]
