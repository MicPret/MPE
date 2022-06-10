#ifndef MPE_LOG_HPP
#define MPE_LOG_HPP

#ifdef NDEBUG

#define MPE_LOG(severity, msg)

#else	//MPE_ENABLE_LOG defined

#include <iostream>

#define MPE_LOG(severity, msg) std::cout << "[" << (severity) << "] [" << __func__  << \
	':' << __LINE__ << "] " << (msg) << '\n'

#endif

#define MPE_INFO(msg) MPE_LOG("INFO", msg)
#define MPE_WARNING(msg) MPE_LOG("WARNING", msg)
#define MPE_ERROR(msg) MPE_LOG("ERROR", msg)

#endif