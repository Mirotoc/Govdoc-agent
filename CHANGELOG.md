# Changelog

## 0.2.0 - 2026-10-20

- Translated all code messages and documentation into English
- Added a `source` field to every finding (`rules` or the model name) and to saved labels
- Added an English sample document next to the Korean one
- Added Linux and macOS socket support and a GitHub Actions build for Windows and Ubuntu
- Added the vibe coding log and agent rules (`CLAUDE.md`)
- Added more tests: masking inside Korean text, source tagging, splitting and trimming
- Removed emoji from all files

## 0.1.0 - 2026-10-20

- First public prototype in standard C++17
- Rule check for resident registration numbers, mobile numbers, and reversed date ranges
- Local LLM review through Ollama on `127.0.0.1`, with strict output validation
- Terminal labeling (accept, edit, reject) saved as JSONL
- Label schema, Ollama Modelfile, and LoRA config draft
