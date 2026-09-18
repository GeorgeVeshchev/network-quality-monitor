#include <iostream>

int main(){
    std::cout << "Hello world!\n";
    std::vector<int> Integers = {1, 2, 3, 4};

    for(int i : Integers){
        std::cout << i << "\n";
    }
}