#include "core.hpp"
#include <fstream>
#include <stdexcept>

std::vector<Block> load_blocks(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) throw std::runtime_error("Cannot open file: " + path);

    std::vector<Block> blocks;
    std::string line, paragraph;
    bool first = true;

    auto flush = [&] {
        if (paragraph.empty()) return;
        blocks.push_back({"p" + std::to_string(blocks.size() + 1), paragraph});
        paragraph.clear();
    };

    while (std::getline(file, line)) {
        if (first && line.rfind("\xEF\xBB\xBF", 0) == 0) line.erase(0, 3);
        first = false;
        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (line.empty()) flush();
        else paragraph += (paragraph.empty() ? "" : " ") + line;
    }
    flush();
    return blocks;
}
