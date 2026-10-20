#include "core.hpp"
#include <sstream>

std::string json_escape(const std::string& s) {
    std::string out;
    for (char c : s) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out += c;
        }
    }
    return out;
}

static void append_utf8(std::string& out, unsigned cp) {
    if (cp < 0x80) {
        out += char(cp);
    } else if (cp < 0x800) {
        out += char(0xC0 | (cp >> 6));
        out += char(0x80 | (cp & 0x3F));
    } else {
        out += char(0xE0 | (cp >> 12));
        out += char(0x80 | ((cp >> 6) & 0x3F));
        out += char(0x80 | (cp & 0x3F));
    }
}

std::string json_unescape(const std::string& s) {
    std::string out;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] != '\\' || i + 1 == s.size()) {
            out += s[i];
            continue;
        }
        char c = s[++i];
        if (c == 'n') out += '\n';
        else if (c == 't') out += '\t';
        else if (c == 'r') out += '\r';
        else if (c == 'u' && i + 4 < s.size()) {
            append_utf8(out, std::stoul(s.substr(i + 1, 4), nullptr, 16));
            i += 4;
        }
        else out += c;
    }
    return out;
}

std::string json_field(const std::string& json, const std::string& key) {
    std::string tag = "\"" + key + "\":\"";
    size_t start = json.find(tag);
    if (start == std::string::npos) return "";
    start += tag.size();

    size_t end = start;
    while (end < json.size() && json[end] != '"') end += json[end] == '\\' ? 2 : 1;
    return json_unescape(json.substr(start, end - start));
}

static std::string trim(const std::string& s) {
    size_t begin = s.find_first_not_of(" \t\r");
    size_t end = s.find_last_not_of(" \t\r");
    return begin == std::string::npos ? "" : s.substr(begin, end - begin + 1);
}

std::vector<std::string> split(const std::string& s, char sep) {
    std::vector<std::string> parts;
    std::stringstream ss(s);
    std::string part;
    while (std::getline(ss, part, sep)) parts.push_back(trim(part));
    return parts;
}
