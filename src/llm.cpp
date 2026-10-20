#include "core.hpp"
#include <algorithm>
#include <stdexcept>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
using SOCKET = int;
#define closesocket close
#endif

static const char* LOCAL_HOST = "127.0.0.1";
static const int OLLAMA_PORT = 11434;

static std::string post_local(const std::string& path, const std::string& body) {
#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(OLLAMA_PORT);
    inet_pton(AF_INET, LOCAL_HOST, &addr.sin_addr);

    if (connect(sock, (sockaddr*)&addr, sizeof addr) != 0) {
        closesocket(sock);
        throw std::runtime_error("Ollama가 켜져 있는지 확인해 주세요 (127.0.0.1:11434)");
    }

    std::string request = "POST " + path + " HTTP/1.0\r\n"
                          "Host: 127.0.0.1\r\n"
                          "Content-Type: application/json\r\n"
                          "Content-Length: " + std::to_string(body.size()) + "\r\n\r\n" + body;
    for (size_t sent = 0; sent < request.size();) {
        int n = send(sock, request.data() + sent, (int)(request.size() - sent), 0);
        if (n <= 0) break;
        sent += n;
    }

    std::string response;
    char buf[4096];
    int n;
    while ((n = recv(sock, buf, sizeof buf, 0)) > 0) response.append(buf, n);
    closesocket(sock);
    return response;
}

static bool is_valid(const std::vector<std::string>& f) {
    static const std::vector<std::string> types = {"SPELL", "TERM", "AMOUNT", "DATE", "MISSING",
                                                   "FORMAT", "PII", "CONSIST", "REGULATION"};
    return f.size() == 6 && f[0].size() > 1 && f[0][0] == 'p' &&
           std::find(types.begin(), types.end(), f[1]) != types.end();
}

std::vector<Issue> ask_llm(const std::vector<Block>& blocks, const std::string& model) {
    std::string doc;
    for (const auto& b : blocks) doc += "[" + b.id + "] " + mask_pii(b.text) + "\n";

    std::string body = "{\"model\":\"" + model + "\",\"stream\":false,\"think\":false,"
                       "\"prompt\":\"" + json_escape(doc) + "\"}";
    std::string reply = post_local("/api/generate", body);
    std::string error = json_field(reply, "error");
    if (!error.empty()) throw std::runtime_error("모델이 응답하지 못했어요: " + error);
    std::string answer = json_field(reply, "response");

    std::vector<Issue> issues;
    for (const auto& line : split(answer, '\n')) {
        auto f = split(line, '|');
        if (!is_valid(f)) continue;
        Issue issue{f[0], f[1], f[2], f[3], f[4], f[5]};
        bool seen = std::any_of(issues.begin(), issues.end(), [&](const Issue& i) {
            return i.block_id == issue.block_id && i.type == issue.type && i.suggestion == issue.suggestion;
        });
        if (!seen) issues.push_back(issue);
    }
    return issues;
}
