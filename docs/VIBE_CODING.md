# Vibe Coding Log

This project was built through **vibe coding**: I described what I wanted in plain language, and an AI coding agent ([Claude Code](https://claude.com/claude-code)) did the research, wrote the code, ran it, and fixed what broke.
My job was to set the direction, the constraints, and the quality bar, and to decide what to keep.

This log records how it actually went, including the wrong turns.

<br>

## Who did what

| I decided | The AI agent did |
|---|---|
| The problem: reviewing government documents without leaking data | Researched local models, Korean regulations, ISO standards, and HWP parsers |
| The core rule: a local LLM only, nothing leaves the PC | Turned that rule into code: the only address is `127.0.0.1` |
| A light model is enough, so use a small Qwen | Compared model sizes and picked 4B as the default |
| Users label the AI's output, and the model learns from it | Designed the label schema and the accept / edit / reject loop |
| Code must be short, comment-free, and dependency-free C++ | Rewrote the prototype from Python to standard C++17 |
| It has to hold up against ISO and government rules | Drafted the compliance checklist |
| Everything in English, for an international portfolio | Translated and reviewed every file |

<br>

## Timeline

### 1. Idea to first draft

My first prompt, roughly:

> I want to build a review program for government project documents. It should use a local AI to find and fix problems, because a cloud LLM would leak the data. Documents are short, so a light Qwen model should be enough. Users label the data and the model learns from it. Labels go into secure storage through a pipeline that cannot leak. It has to follow ISO and government procedures so it does not become a legal problem later. Research the technical parts and fill them in.

The agent came back with a Python prototype: Ollama for the model, PyMuPDF and python-docx for parsing, `mss` for screen capture, and Label Studio for labeling.
It worked on paper, but it pulled in a lot of third-party software.

### 2. The pivot to plain C++

Before anything shipped, I changed the brief:

> It should work as a portfolio piece that people can read. Keep the code as short as possible and easy to follow for whoever debugs it, and remove the comments. Avoid piling on plugins and libraries. Implement it in very basic, intuitive C++.

The agent threw away the Python code and rewrote the core in **standard C++17 with no third-party libraries**:

- HTTP to Ollama over a raw socket instead of an HTTP library
- A minimal JSON string reader instead of a JSON library
- Labeling in the terminal instead of Label Studio
- One file per responsibility, with `core.hpp` as the map

The result is 378 lines. For a tool meant for security-sensitive offices, this matters: every dependency is one more thing someone has to approve.

### 3. Testing against a real model found real bugs

The agent did not stop at "it compiles". It built a temporary model from the Qwen3 4B already installed on my machine, ran the tool against it, and deleted the temporary model afterwards. That surfaced three problems:

| Problem | What happened | Fix |
|---|---|---|
| Silent failure | When the model name did not exist, Ollama returned an error and the tool printed only the rule findings, with no warning | Read Ollama's `error` field and stop with a clear message |
| Leaked reasoning | Qwen3 wrote its chain of thought into the answer, and lines from it were parsed as findings | Accept only lines with a valid block ID and a known error type, then de-duplicate |
| Korean text in MSVC | Korean string literals broke the Windows build | Compile with `/utf-8` |

That middle row is my favorite lesson: **never trust the format of LLM output, validate it.**

### 4. Shipping it

Getting the first push to GitHub took more attempts than writing the code:

- A token pasted into a chat window is a leaked token. It turned out to be invalid anyway, and was revoked.
- A token generated while the browser was signed into a different account belongs to that account. GitHub answered `Permission denied to <other account>`.
- A fine-grained token without **Contents: Read and write** authenticates fine but cannot push (`403`).
- A classic token with the `public_repo` scope worked on the first try.

None of this is in the code, but all of it is part of building software with AI. The agent can explain each error, but the credentials stay in the human's hands.

### 5. Going international

Last, I asked for every file to be in English and for a full review pass. The review added the `source` field (so you can tell a rule finding from a model finding), an English sample document, Linux support and a CI build, this log, and [`CLAUDE.md`](../CLAUDE.md).

<br>

## How I work with the agent

What worked for me, as a first-time vibe coder:

1. **Say the constraint before the feature.** "Nothing leaves the PC" shaped every later decision better than any feature request did.
2. **Ask for it to be run, not just written.** The worst bugs only appeared against a real model.
3. **Change direction early.** Switching from Python to C++ was cheap because it happened before anything shipped.
4. **Keep the rules in the repository.** [`CLAUDE.md`](../CLAUDE.md) tells any future AI session the same rules I told this one.
5. **Stay the reviewer.** The AI suggests and I decide. That is also how the tool itself is designed.

<br>

## What I would do next

- Measure the hardware estimates instead of guessing
- Read HWPX directly (it is a zip of XML files)
- Collect the first 300 labels and run a real LoRA experiment
- Record a short terminal demo
