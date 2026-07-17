#pragma once

#define FMT_HEADER_ONLY
#define FMT_USE_EXCEPTIONS 0
#define FMT_USE_LOCALE 0 //disables locales
#define FMT_OPTIMIZE_SIZE 2 //disables some unicode features, named arguments
#define FMT_USE_CONSTEVAL 1
#define FMT_LMN_USE_FLOAT 1
#define FMT_LMN_USE_DOUBLE 0
#define FMT_LMN_USE_LONG_DOUBLE 0
#include <fmt/format.h>
