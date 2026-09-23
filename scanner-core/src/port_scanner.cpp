#include "port_scanner.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <cerrno>
#include <future>
#include <memory>

namespace scanner{
    PortScanner::PortScanner(size_t thread_count) : pool_(thread_count) {}

    bool PortScanner::is_port_open(const std::string& ip, uint16_t port, int timeout_ms){
        int sock_fd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (sock_fd < 0){
            return false;
        }

        int flags = ::fcntl(sock_fd, F_GETFL, 0);
        if(flags < 0){
            ::close(sock_fd);
            return false;
        }

        if(::fcntl(sock_fd, F_SETFL, flags | O_NONBLOCK) < 0){
            ::close(sock_fd);
            return false;
        }

        sockaddr_in addr = {};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        if(::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0){
            ::close(sock_fd);
            return false;
        }

        int res = ::connect(sock_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
        if (res < 0 && errno != EINPROGRESS){
            ::close(sock_fd);
            return false;
        } else if(res == 0){
            ::close(sock_fd);
            return true;
        }

        fd_set set = {};
        FD_ZERO(&set);
        FD_SET(sock_fd, &set);

        timeval tv;
        tv.tv_sec = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;

        res = ::select(sock_fd + 1, nullptr, &set, nullptr, &tv);

        if(res <= 0){
            ::close(sock_fd);
            return false;
        }

        int error = 0;
        socklen_t len = sizeof(error);
        if (::getsockopt(sock_fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0 || error != 0){
            ::close(sock_fd);
            return false;
        }   

        ::close(sock_fd);
        return true;
        
    }

    std::vector<std::future<ScanResult>> PortScanner::scan_range(
    const std::string& ip, 
    uint16_t start_port, 
    uint16_t end_port, 
    int timeout_ms) 
    {
    std::vector<std::future<ScanResult>> results;
    results.reserve(end_port - start_port + 1);

    for(uint32_t port = start_port; port <= end_port; ++port) {
    auto promise = std::make_shared<std::promise<ScanResult>>();
    results.push_back(promise->get_future());

    pool_.enqueue([this, ip, port, timeout_ms, promise]() {
        bool open = is_port_open(ip, static_cast<uint16_t>(port), timeout_ms);
        promise->set_value(ScanResult{static_cast<uint16_t>(port), open});
        });
    }

    return results;
    }

 }       


