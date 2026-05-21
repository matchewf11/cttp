#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdio.h>

#define LOG_ERROR 0
#define LOG_WARN 1
#define LOG_INFO 2

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_INFO
#endif

#if LOG_LEVEL <= LOG_WARN
#define logw(...) log_warn(__VA_ARGS__)
#else
#define logw(...)
#endif

#if LOG_LEVEL <= LOG_INFO
#define logi(...) log_info(__VA_ARGS__)
#else
#define logi(...)
#endif

#if LOG_LEVEL <= LOG_ERROR
#define loge(...) log_error(__VA_ARGS__)
#else
#define loge(...)
#endif

static inline void log_error(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("[ERROR] ");
    vprintf(fmt, ap);
    printf("\n");
    va_end(ap);
}

static inline void log_warn(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("[WARN] ");
    vprintf(fmt, ap);
    printf("\n");
    va_end(ap);
}

static inline void log_info(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("[INFO] ");
    vprintf(fmt, ap);
    printf("\n");
    va_end(ap);
}

#endif
