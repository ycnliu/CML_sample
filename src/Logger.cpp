#include "Logger.h"
#include <iostream>

// Define the static storage for log messages
std::vector<std::string> Logger::messages;

void Logger::log(const std::string& message) {
    messages.push_back(message);
    // Also print to console (could be directed to a file or GUI in real system)
    std::cout << message << std::endl;
}

const std::vector<std::string>& Logger::getLogs() const {
    return messages;
}

void Logger::clear() {
    messages.clear();
}
