#pragma once
#include <string>
#include <vector>

struct Block {
    std::string id;
    std::string text;
};

struct Issue {
    std::string block_id;
    std::string type;
    std::string severity;
    std::string evidence;
    std::string suggestion;
    std::string reason;
    std::string source;
};

std::vector<Block> load_blocks(const std::string& path);

std::string mask_pii(const std::string& text);
std::vector<Issue> check_rules(const std::vector<Block>& blocks);

std::vector<Issue> ask_llm(const std::vector<Block>& blocks, const std::string& model);

void label_issues(const std::vector<Issue>& issues, const std::string& out_path);

std::string json_escape(const std::string& s);
std::string json_unescape(const std::string& s);
std::string json_field(const std::string& json, const std::string& key);
std::vector<std::string> split(const std::string& s, char sep);
