#ifndef EDATASTREAM_EXT_H
#define EDATASTREAM_EXT_H

// These are defined because Qt does not define them.
template <> inline float qbswap<float>(float source) { return source; }
template <> inline double qbswap<double>(double source) { return source; }

#endif
