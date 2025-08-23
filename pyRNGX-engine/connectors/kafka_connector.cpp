#include <string>
#include <iostream>

class KafkaConnector {
public:
    bool connect(const std::string& brokers) {
        std::cout << "[kafka] connect to " << brokers << "\n";
        return true;
    }
    bool publish(const std::string& topic, const std::string& msg) {
        std::cout << "[kafka] publish " << topic << ": " << msg << "\n";
        return true;
    }
    void close() { std::cout << "[kafka] close\n"; }
};
