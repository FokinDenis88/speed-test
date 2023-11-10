#ifndef REPEAT_MACROS_HPP
#define REPEAT_MACROS_HPP

#define REPEAT2(x) x x
#define REPEAT4(x) REPEAT2(x) REPEAT2(x)
#define REPEAT8(x) REPEAT4(x) REPEAT4(x)
#define REPEAT16(x) REPEAT8(x) REPEAT8(x)
#define REPEAT32(x) REPEAT16(x) REPEAT16(x)
#define REPEAT64(x) REPEAT32(x) REPEAT32(x)
#define REPEAT128(x) REPEAT64(x) REPEAT64(x)
#define REPEAT256(x) REPEAT128(x) REPEAT128(x)
#define REPEAT512(x) REPEAT256(x) REPEAT256(x)
#define REPEAT1024(x) REPEAT512(x) REPEAT512(x)
#define REPEAT2048(x) REPEAT1024(x) REPEAT1024(x)
#define REPEAT4096(x) REPEAT2048(x) REPEAT2048(x)
#define REPEAT8192(x) REPEAT4096(x) REPEAT4096(x)

#endif // !REPEAT_MACROS_HPP
