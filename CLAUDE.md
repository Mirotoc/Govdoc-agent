# Rules for AI coding agents

This repository is developed through vibe coding. Any AI agent working here (Claude Code or otherwise) must follow these rules.

## Non-negotiable

- The tool talks to `127.0.0.1` only. Never add an option, environment variable, or fallback that could reach another host.
- Personal data is masked with `mask_pii` before any text is sent to the model.
- Never commit real documents, labels, or model weights. `data/`, `adapters/`, and document formats are in `.gitignore` for a reason.
- No sample may contain real personal data. Use obviously fictional names and numbers.

## Code style

- Standard C++17 only. Do not add third-party libraries. Use operating system APIs only when the standard library cannot do the job.
- No comments. Make the code readable through names and short functions instead.
- One file, one responsibility. Every public function is declared in `src/core.hpp`.
- Keep it short. If a change makes a file noticeably longer, look for a simpler way first.
- User-facing messages are short, plain English, and tell the user what to do next.

## Working rules

- Build and run the tests before saying a change is done: `cmake --build build --config Release` and `ctest --test-dir build -C Release`.
- When the change touches the model, run it against a real local model, not just the tests.
- Never trust model output. Validate its format before using it.
- Update `README.md` (code tour line counts included) and `CHANGELOG.md` when behavior changes.
- No emoji in code, docs, or commit messages.
