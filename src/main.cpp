#include "core.hpp"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

static void print(const Issue& i) {
    std::cout << "[" << i.block_id << "] " << i.type << " (" << i.severity << ")\n"
              << "  evidence:   " << i.evidence << "\n"
              << "  suggestion: " << i.suggestion << "\n"
              << "  reason:     " << i.reason << "\n"
              << "  source:     " << i.source << "\n\n";
}

int main(int argc, char** argv) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    if (argc < 2) {
        std::cout << "Usage: govdoc <document.txt> [--rules-only] [--label] [--model name]\n";
        return 1;
    }

    std::string model = "govdoc-reviewer";
    bool rules_only = false, label = false;
    for (int k = 2; k < argc; ++k) {
        std::string arg = argv[k];
        if (arg == "--rules-only") rules_only = true;
        if (arg == "--label") label = true;
        if (arg == "--model" && k + 1 < argc) model = argv[++k];
    }

    try {
        auto blocks = load_blocks(argv[1]);
        auto issues = check_rules(blocks);
        if (!rules_only) {
            auto ai = ask_llm(blocks, model);
            issues.insert(issues.end(), ai.begin(), ai.end());
        }

        std::cout << "Found " << issues.size() << " item(s) to review.\n"
                  << "These are suggestions. You make the final call.\n\n";
        for (const auto& i : issues) print(i);

        if (label) label_issues(issues, "data/labels.jsonl");
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
