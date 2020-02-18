#ifndef global_HPP
#define global_HPP

/**
 * This file contains utilities that would be useful across all files. Any file is encouraged to include this header.
 * But extreme caution should be taken before adding items here. We don't want any bloat.
 */

/// Types

#include <cstdint>

#ifndef uint
typedef uint32_t uint;
#endif
typedef int32_t sint;

#endif