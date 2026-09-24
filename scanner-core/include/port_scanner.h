#pragma once
#include <string>
#include <vector>
#include <cstdlib>
#include <future>
#include <nlohmann/json.hpp>
#include "thread_pool.h"

namespace scanner{
    struct ScanResult{
        uint16_t port;
        bool is_open;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ScanResult, port, is_open)   

    class PortScanner{
        public:
        explicit PortScanner(size_t thread_count);
        bool is_port_open(const std::string& ip, uint16_t port, int timeout_ms = 500);
        std::vector<std::future<ScanResult>> scan_range(
        const std::string& ip, 
        uint16_t start_port, 
        uint16_t end_port, 
        int timeout_ms = 500
        );
        
        private:
        ThreadPool pool_;

    };
}