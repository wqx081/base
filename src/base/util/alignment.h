// Macros for dealing with memory alignment.
#ifndef BASE_UTIL_ALIGNMENT_H
#define BASE_UTIL_ALIGNMENT_H

// Round down 'x' to the nearest 'align' boundary
#define MPR_ALIGN_DOWN(x, align) ((x) & (-(align)))

// Round up 'x' to the nearest 'align' boundary
#define MPR_ALIGN_UP(x, align) (((x) + ((align) - 1)) & (-(align)))

#endif
