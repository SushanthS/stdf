/*
 * Copyright (C) 2025 ComputingStudios
 * 
 * Project Director: Sushanth Sivaram
 * Author: AI Assistant (Claude-4-Sonnet) with Cursor IDE
 * Generated: July 2025
 * 
 * This file is part of the STDF Parser project, a high-performance C++ library
 * for parsing Standard Test Data Format files used in semiconductor testing.
 *
 * This software was generated using Cursor IDE with Claude-4-Sonnet AI assistance,
 * under the direction of Sushanth Sivaram, providing advanced code generation 
 * and optimization capabilities.
 *
 * Licensed under the MIT License. See LICENSE file for details.
 * 
 * Description: Syslog integration wrapper
 *              Professional logging system with severity levels and stream support
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <syslog.h>
#include <string>
#include <sstream>

namespace STDF {

class Logger {
public:
    static void init(const std::string& ident) {
        openlog(ident.c_str(), LOG_PID | LOG_CONS, LOG_USER);
    }
    
    static void cleanup() {
        closelog();
    }
    
    static void info(const std::string& message) {
        syslog(LOG_INFO, "%s", message.c_str());
    }
    
    static void warning(const std::string& message) {
        syslog(LOG_WARNING, "%s", message.c_str());
    }
    
    static void error(const std::string& message) {
        syslog(LOG_ERR, "%s", message.c_str());
    }
    
    static void debug(const std::string& message) {
        syslog(LOG_DEBUG, "%s", message.c_str());
    }
    
    // Utility class for stream-like logging
    class LogStream {
    private:
        std::ostringstream oss;
        int priority;
        
    public:
        LogStream(int prio) : priority(prio) {}
        
        template<typename T>
        LogStream& operator<<(const T& value) {
            oss << value;
            return *this;
        }
        
        // Special handling for std::endl and other manipulators
        LogStream& operator<<(std::ostream& (*manip)(std::ostream&)) {
            if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {
                flush();
                return *this;
            }
            oss << manip;
            return *this;
        }
        
        ~LogStream() {
            flush();
        }
        
        void flush() {
            std::string message = oss.str();
            if (!message.empty() && message.back() == '\n') {
                message.pop_back(); // Remove trailing newline for syslog
            }
            if (!message.empty()) {
                syslog(priority, "%s", message.c_str());
            }
            oss.str("");
            oss.clear();
        }
    };
    
    static LogStream log_info() { return LogStream(LOG_INFO); }
    static LogStream log_warning() { return LogStream(LOG_WARNING); }
    static LogStream log_error() { return LogStream(LOG_ERR); }
    static LogStream log_debug() { return LogStream(LOG_DEBUG); }
};

// Convenience macros for easy replacement (using different names to avoid syslog.h conflicts)
#define STDF_LOG_INFO STDF::Logger::log_info()
#define STDF_LOG_WARNING STDF::Logger::log_warning()
#define STDF_LOG_ERROR STDF::Logger::log_error()
#define STDF_LOG_DEBUG STDF::Logger::log_debug()

} // namespace STDF

#endif // LOGGER_H