#include "../src/core.hpp"
#include <iostream>

static int failed = 0;

static void expect(bool ok, const char* name) {
    std::cout << (ok ? "통과  " : "실패  ") << name << "\n";
    if (!ok) ++failed;
}

int main() {
    expect(mask_pii("번호 900101-1234567") == "번호 900101-*******", "주민등록번호 가리기");
    expect(mask_pii("연락처 010-1234-5678") == "연락처 010-****-****", "휴대전화 번호 가리기");

    auto issues = check_rules({{"p1", "사업기간: 2027.03.01 ~ 2026.12.31"},
                               {"p2", "사업기간: 2027.03.01 ~ 2027.12.31"}});
    expect(issues.size() == 1 && issues[0].type == "DATE" && issues[0].block_id == "p1", "기간 역전 찾기");

    expect(json_unescape("\\u003c\\n\\\"") == "<\n\"", "JSON 문자열 해석");
    expect(json_field("{\"response\":\"p1|SPELL\"}", "response") == "p1|SPELL", "응답 필드 꺼내기");

    return failed;
}
