#include "core.hpp"
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

static std::string now() {
    std::time_t t = std::time(nullptr);
    char buf[32];
    std::strftime(buf, sizeof buf, "%Y-%m-%dT%H:%M:%S", std::localtime(&t));
    return buf;
}

static std::string kv(const std::string& key, const std::string& value) {
    return "\"" + key + "\":\"" + json_escape(value) + "\"";
}

void label_issues(const std::vector<Issue>& issues, const std::string& out_path, const std::string& model) {
    std::filesystem::create_directories(std::filesystem::path(out_path).parent_path());
    std::ofstream out(out_path, std::ios::app | std::ios::binary);
    if (!out) throw std::runtime_error("라벨 파일을 열 수 없어요: " + out_path);

    for (const auto& i : issues) {
        std::cout << "[" << i.block_id << "] " << i.type << " · " << i.evidence << "\n"
                  << "  제안: " << i.suggestion << "\n"
                  << "  승인 a / 수정 e / 거절 r / 건너뛰기 Enter > ";

        std::string key;
        std::getline(std::cin, key);
        std::string verdict = key == "a" ? "ACCEPT" : key == "e" ? "EDIT" : key == "r" ? "REJECT" : "";
        if (verdict.empty()) continue;

        std::string fix = verdict == "REJECT" ? "" : i.suggestion;
        if (verdict == "EDIT") {
            std::cout << "  수정안 > ";
            std::getline(std::cin, fix);
        }

        out << "{" << kv("block_id", i.block_id) << "," << kv("error_type", i.type) << ","
            << kv("severity", i.severity) << "," << kv("evidence", i.evidence) << ","
            << kv("ai_suggestion", i.suggestion) << "," << kv("final_correction", fix) << ","
            << kv("verdict", verdict) << "," << kv("model_version", model) << ","
            << kv("created_at", now()) << "}\n";
    }
    std::cout << "라벨을 저장했어요: " << out_path << "\n";
}
