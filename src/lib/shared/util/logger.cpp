#include "logger.h"

zlog_category_t *logger_zc = NULL;

void logger_instance(const char* cat) {
	logger_zc = zlog_get_category(cat);
}
