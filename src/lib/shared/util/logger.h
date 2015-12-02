#ifndef _LOGGER_H_
#define _LOGGER_H_

#include "zlog.h"

extern zlog_category_t *logger_zc;

#define LOGGER_INIT(conf, cat)					\
	if (zlog_init(conf)) { exit(-1); }			\
	logger_instance(cat);

#define LOGGER_FINI					zlog_fini();

#define LOGGER_DEBUG(format, ...)	zlog_debug(logger_zc, format, ##__VA_ARGS__)
#define LOGGER_INFO(format, ...)	zlog_info(logger_zc, format, ##__VA_ARGS__)
#define LOGGER_NOTICE(format, ...)	zlog_notice(logger_zc, format, ##__VA_ARGS__)
#define LOGGER_WARN(format, ...)	zlog_warn(logger_zc, format, ##__VA_ARGS__)
#define LOGGER_ERROR(format, ...)	zlog_error(logger_zc, format, ##__VA_ARGS__)
#define LOGGER_FATAL(format, ...)	zlog_fatal(logger_zc, format, ##__VA_ARGS__)

void logger_instance(const char* cat);

#endif //LOGGER_H_
