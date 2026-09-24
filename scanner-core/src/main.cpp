#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "port_scanner.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(int argc, char* argv[]){
    std::string target_ip = "127.0.0.1";
    uint16_t start_port = 1;
    uint16_t end_port = 1024;
    int threads_count = 8;
    int timeout_ms = 200;

    // 2. Удобный парсинг флагов из командной строки
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--threads" && i + 1 < argc) {
            threads_count = std::stoi(argv[++i]);
        } else if (arg == "--ip" && i + 1 < argc) {
            target_ip = argv[++i];
        } else if (arg == "--start-port" && i + 1 < argc) {
            start_port = static_cast<uint16_t>(std::stoi(argv[++i]));
        } else if (arg == "--end-port" && i + 1 < argc) {
            end_port = static_cast<uint16_t>(std::stoi(argv[++i]));
        } else if (arg == "--timeout" && i + 1 < argc) {
            timeout_ms = std::stoi(argv[++i]);
        }
    }

    auto start_time = std::chrono::high_resolution_clock::now();

    scanner::PortScanner scanner(threads_count);
    auto futures = scanner.scan_range(target_ip, start_port, end_port, timeout_ms);

    json results_array = json::array();

    for(auto& fut : futures){
        scanner::ScanResult res = fut.get();
        if(res.is_open){
            results_array.push_back(res);
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto total_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

    json final_output;
    final_output["target_ip"] = target_ip;
    final_output["total_time_ms"] = total_time_ms;
    final_output["threads_used"] = threads_count;
    final_output["open_ports"] = results_array;

    std::cout << final_output.dump(4) << std::endl;

    return 0;


}
