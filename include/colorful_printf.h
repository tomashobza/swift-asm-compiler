// ANSI escape codes for setting text color
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

// Macro for colored printf
#define printf_red(format, ...) printf(ANSI_COLOR_RED format ANSI_COLOR_RESET, ##__VA_ARGS__)
#define printf_green(format, ...) printf(ANSI_COLOR_GREEN format ANSI_COLOR_RESET, ##__VA_ARGS__)
#define printf_yellow(format, ...) printf(ANSI_COLOR_YELLOW format ANSI_COLOR_RESET, ##__VA_ARGS__)
#define printf_blue(format, ...) printf(ANSI_COLOR_BLUE format ANSI_COLOR_RESET, ##__VA_ARGS__)
#define printf_magenta(format, ...) printf(ANSI_COLOR_MAGENTA format ANSI_COLOR_RESET, ##__VA_ARGS__)
#define printf_cyan(format, ...) printf(ANSI_COLOR_CYAN format ANSI_COLOR_RESET, ##__VA_ARGS__)
