//
// Created by nexie on 16.06.2025.
//

#ifndef LOGGER_H
#define LOGGER_H

// #include "nx_string_extension.h"
#include "marker_string.h"
#include <fstream>

namespace nx {

    enum class log_level {
        ll_debug = 1,
        ll_info,
        ll_warn,
        ll_error,
        ll_fatal,
    };

    struct log_context {
        const char * file;
        const int line;
        const char * function;
    };

    struct log_message {
        log_level level;
        marker_string msg;
        log_context context;
    };

    struct log_category {
        char * name;
        FILE * file;
        log_level upper_level {log_level::ll_fatal};
        log_level lower_level {log_level::ll_info};
    };


    class logger {
        static logger * _instance;
        // nx::forward_list<log_category> _categories;
        std::ofstream _log_stream;

        ~logger();

        public:
        static logger & instance ();
        static void free_instance ();
        marker_string create_message(log_level level, const marker_string & msg, log_context context);
        void write_message(const marker_string & message);

    public:
        void add_log(const log_message & msg);
        void set_log_file(const marker_string & file);
    };

#define log_info(message)   nx::logger::instance().add_log({nx::log_level::ll_info, message, {__FILE__, __LINE__, __PRETTY_FUNCTION__}})
#define log(message)        nx::logger::instance().add_log({nx::log_level::ll_debug, message,{__FILE__, __LINE__, __PRETTY_FUNCTION__}})
#define log_warn(message)   nx::logger::instance().add_log({nx::log_level::ll_warn, message, {__FILE__, __LINE__, __PRETTY_FUNCTION__}})
#define log_error(message)  nx::logger::instance().add_log({nx::log_level::ll_error, message, {__FILE__, __LINE__, __PRETTY_FUNCTION__}})
#define log_fatal(message)  nx::logger::instance().add_log({nx::log_level::ll_fatal, message, {__FILE__, __LINE__, __PRETTY_FUNCTION__}})

} // nx

#endif //LOGGER_H
