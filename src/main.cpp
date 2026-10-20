#include "core.hpp"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

static void print(const Issue& i) {
    std::cout << "[" << i.block_id << "] " << i.type << " (" << i.severity << ")\n"
              << "  근거: " << i.evidence << "\n"
              << "  제안: " << i.suggestion << "\n"
              << "  이유: " << i.reason << "\n\n";
}

int main(int argc, char** argv) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    if (argc < 2) {
        std::cout << "사용법: govdoc <문서.txt> [--rules-only] [--label] [--model 이름]\n";
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

        std::cout << "확인이 필요한 곳을 " << issues.size() << "개 찾았어요.\n"
                  << "AI가 만든 제안이에요. 최종 판단은 담당자가 해 주세요.\n\n";
        for (const auto& i : issues) print(i);

        if (label) label_issues(issues, "data/labels.jsonl", model);
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}
