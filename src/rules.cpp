#include "core.hpp"
#include <regex>

static const std::regex rrn(R"(\b(\d{6})-?[1-4]\d{6}\b)");
static const std::regex phone(R"(\b01[016789]-?\d{3,4}-?\d{4}\b)");
static const std::regex period(R"((\d{4})\.(\d{1,2})\.(\d{1,2})\s*~\s*(\d{4})\.(\d{1,2})\.(\d{1,2}))");

std::string mask_pii(const std::string& text) {
    std::string masked = std::regex_replace(text, rrn, "$1-*******");
    return std::regex_replace(masked, phone, "010-****-****");
}

static int date_key(const std::smatch& m, int from) {
    return std::stoi(m[from].str()) * 10000 + std::stoi(m[from + 1].str()) * 100 + std::stoi(m[from + 2].str());
}

std::vector<Issue> check_rules(const std::vector<Block>& blocks) {
    std::vector<Issue> issues;
    for (const auto& b : blocks) {
        std::smatch m;
        if (std::regex_search(b.text, m, rrn))
            issues.push_back({b.id, "PII", "critical", mask_pii(m.str()),
                              "Remove the resident registration number", "A resident registration number is exposed", "rules"});
        if (std::regex_search(b.text, m, phone))
            issues.push_back({b.id, "PII", "critical", mask_pii(m.str()),
                              "Mask the personal phone number", "A mobile phone number is exposed", "rules"});

        for (std::sregex_iterator it(b.text.begin(), b.text.end(), period), end; it != end; ++it)
            if (date_key(*it, 4) < date_key(*it, 1))
                issues.push_back({b.id, "DATE", "high", it->str(),
                                  "Check the end date", "The end date is earlier than the start date", "rules"});
    }
    return issues;
}
