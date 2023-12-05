
# Define ANSI escape codes for colors
RED = '\033[91m'
GREEN = '\033[92m'
YELLOW = '\033[93m'
BLUE = '\033[94m'
MAGENTA = '\033[95m'
CYAN = '\033[96m'
WHITE = '\033[97m'
RESET = '\033[0m'

def print_red(text):
    print(f"{RED}{text}{RESET}")

def print_green(text):
    print(f"{GREEN}{text}{RESET}")

def print_yellow(text):
    print(f"{YELLOW}{text}{RESET}")

def print_blue(text):
    print(f"{BLUE}{text}{RESET}")

def print_magenta(text):
    print(f"{MAGENTA}{text}{RESET}")

def print_cyan(text):
    print(f"{CYAN}{text}{RESET}")

def print_white(text):
    print(f"{WHITE}{text}{RESET}")