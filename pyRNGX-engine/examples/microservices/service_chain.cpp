#include <iostream>
#include <string>

std::string serviceA(const std::string& s) { return "A(" + s + ")"; }
std::string serviceB(const std::string& s) { return "B(" + s + ")"; }
std::string serviceC(const std::string& s) { return "C(" + s + ")"; }

int main() {
    std::string payload = "payload";
    auto a = serviceA(payload);
    auto b = serviceB(a);
    auto c = serviceC(b);
    std::cout << c << std::endl;
    return 0;
}
