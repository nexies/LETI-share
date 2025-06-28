//
// Created by nexie on 16.06.2025.
//

#include "logger.h"
// #include "nx_iostream.h"
#include <mutex>


namespace nx {
    std::mutex logger_mutex;
    logger* logger::_instance { nullptr };

    logger::~logger() {
        _log_stream.close();
    }

    logger & logger::instance() {
        if (!_instance)
            _instance = new logger();
        return *_instance;
    }

    void logger::free_instance() {
        if (_instance)
            delete _instance;
    }

    marker_string logger::create_message(log_level level, const marker_string &msg, log_context context) {
        std::string out;
        switch (level) {
            case log_level::ll_warn:
                out += "warn: ";
                break;
                case log_level::ll_error:
                out += "error: ";
                break;
                case log_level::ll_info:
                out += "info: ";
                break;
                case log_level::ll_fatal:
                out += "fatal error: ";
                break;
                default:
                break;
        }
        out += "[";
        out += context.function;
        out += "] - ";
        out += msg.c_str();
        out += "\n";
        return marker_string(out);
    }

    void logger::write_message(const marker_string &message) {
        std::lock_guard<std::mutex> lock(logger_mutex);
        _log_stream << message << std::flush;

    }

    void logger::add_log(const log_message &msg) {
        auto str = create_message(msg.level, msg.msg, msg.context);
        write_message(str);
    }

    void logger::set_log_file(const marker_string & file) {
        _log_stream.close();
        _log_stream.open(file.c_str(), std::ios::app);
    }
} // nx