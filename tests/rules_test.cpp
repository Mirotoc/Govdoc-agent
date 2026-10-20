#include "../src/core.hpp"
#include <iostream>

static int failed = 0;

static void expect(bool ok, const char* name) {
    std::cout << (ok ? "PASS  " : "FAIL  ") << name << "\n";
    if (!ok) ++failed;
}

int main() {
    expect(mask_pii("ID 900101-1234567") == "ID 900101-*******", "masks resident registration number");
    expect(mask_pii("mobile 010-1234-5678") == "mobile 010-****-****", "masks mobile phone number");
    expect(mask_pii("연락처 010-1234-5678") == "연락처 010-****-****", "masks inside Korean text");

    auto issues = check_rules({{"p1", "Period: 2027.03.01 ~ 2026.12.31"},
                               {"p2", "Period: 2027.03.01 ~ 2027.12.31"}});
    expect(issues.size() == 1 && issues[0].type == "DATE" && issues[0].block_id == "p1", "finds reversed period");
    expect(issues.size() == 1 && issues[0].source == "rules", "marks rule findings by source");

    expect(json_unescape("\\u003c\\n\\\"") == "<\n\"", "unescapes JSON strings");
    expect(json_field("{\"response\":\"p1|SPELL\"}", "response") == "p1|SPELL", "reads a JSON field");
    expect(split(" a | b ", '|') == std::vector<std::string>{"a", "b"}, "splits and trims");

    return failed;
}
