#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>

// Logger for recording system events and states
class Logger {
private:
    static std::vector<std::string> messages;
public:
    Logger() = default;
    // Log a message (store it and output to console)
    void log(const std::string& message);
    // Retrieve all logged messages
    const std::vector<std::string>& getLogs() const;
    // Clear all logged messages
    void clear();
};

#endif // LOGGER_H
