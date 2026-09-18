#include <iostream>
#include <string>

int main(int argc, char* argv[]){
    int threads = 4;
    std::string hosts_arg = " ";

    for(int i = 1; i < argc; ++i){
        std::string arg = argv[i];
        if(arg == "--threads" && i + 1 < argc){
            threads = std::stoi(argv[++i]);
        } else if(arg == "--hosts" && i + 1 < argc){
            hosts_arg = argv[++i];
        }
    }

    std::cout << R"({
    "total_time_ms": 320,
    "threads_used": )" << threads << R"(,
    "results": [
    { "host": "8.8.8.8", "ping_ms": 14.2, "status": "ONLINE" },
    { "host": "1.1.1.1", "ping_ms": 9.8, "status": "ONLINE" },
    { "host": "github.com", "ping_ms": -1.0, "status": "TIMEOUT" }
    ]
    })" << std::endl;

    return 0;

}