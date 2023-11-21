#ifndef DEBUG_H
#define DEBUG_H

//! DEBUG
#define DEBUG_PSA 0
#if DEBUG && DEBUG_PSA
#define DEBUG_CODE(code) \
    do                   \
    {                    \
        code             \
    } while (0)
#else
#define DEBUG_CODE(code) \
    do                   \
    {                    \
    } while (0)
#endif

#endif // DEBUG_H