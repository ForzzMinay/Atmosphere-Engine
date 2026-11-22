# 🌌 Atmosphere Engine

[![Build Status](https://img.shields.io/github/actions/workflow/status/yourusername/atmosphere-engine/ci.yml?branch=main&logo=github)](https://github.com/yourusername/atmosphere-engine/actions)
[![Python](https://img.shields.io/badge/Python-3.11+-blue?logo=python&logoColor=white)](https://www.python.org/)
[![FastAPI](https://img.shields.io/badge/FastAPI-0.100+-009688?logo=fastapi&logoColor=white)](https://fastapi.tiangolo.com/)
[![Docker](https://img.shields.io/badge/Docker-Ready-2496ED?logo=docker&logoColor=white)](https://www.docker.com/)
[![License](https://img.shields.io/badge/License-MIT-green)](LICENSE)

**Atmosphere Engine** — это интеллектуальный бэкенд-сервис для анализа и поиска музыки, который выходит за рамки стандартных жанровых тегов. Движок оценивает треки по «глубине» текстов, эмоциональному окрасу и аудио-характеристикам, помогая находить музыку с плотной атмосферой и смысловой нагрузкой (в духе 3umph, tewiq, Поларойд и андеграунд-сцены).

## ✨ Ключевые возможности

- 🧠 **NLP-анализ текстов:** Оценка смысловой нагрузки, рефлексии, меланхолии и социального подтекста с помощью локальных LLM и HuggingFace моделей.
- 🎧 **Аудио-профилирование:** Интеграция со Spotify Web API для извлечения audio features (valence, energy, acousticness, danceability).
- 📡 **Underground Radar:** Парсинг афиш небольших площадок и нишевых Telegram-каналов для отслеживания первых концертов и андеграунд-ивентов.
- 🎯 **Vibe-Matching API:** Эндпоинты для поиска треков и событий по кастомным векторам «вайба» (например, `melancholy: 0.8, deep_lyrics: 0.9, energy: 0.3`).

## 🏗 Архитектура

Проект построен по принципам Clean Architecture и разделен на независимые слои.

```mermaid
graph TD
    A[Client / Telegram Bot] -->|REST / Webhook| B(API Gateway - FastAPI)
    B --> C{Vibe Engine}
    C -->|Audio Features| D[Spotify API]
    C -->|Lyrics NLP| E[HuggingFace / Local LLM]
    C -->|Events| F[Underground Radar Parser]
    B --> G[(PostgreSQL)]
    B --> H[(Redis Cache)]
    F -->|Celery Workers| I[Message Broker]
