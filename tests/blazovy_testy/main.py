import os, subprocess
from dataclasses import dataclass
from typing import List

PROJ_ROOT = os.path.abspath(os.path.dirname(os.path.realpath(__file__)) + "/..")

TESTS_DIR = os.path.join(PROJ_ROOT, "test/tests")
COMPILER_PATH = os.path.join(PROJ_ROOT, "comp")
TMP_PATH = os.path.join(PROJ_ROOT, "tmp")

if (os.name == "nt"):
    INTERPRETER_PATH = os.path.abspath(os.path.join(PROJ_ROOT, "ic23int_win_2023-10-04/ic23int.exe"))
else:
    INTERPRETER_PATH = os.path.abspath(os.path.join(PROJ_ROOT, "ic23int_linux_2023-10-04/ic23int"))

# INTERPRETER_PATH = os.path.abspath(os.path.join(PROJ_ROOT, "interpreter_simulator/error"))

@dataclass
class Test:
    in_file_path: str
    out_file_path: str

def print_color_err(message: str):
    print("\033[91m" + message + "\033[0m")

def print_color_ok(message: str):
    print("\033[92m" + message + "\033[0m")

def get_tests_from_dir(dir_path) -> List[Test]:
    tests = []

    for file_name in os.listdir(dir_path):
        if file_name.endswith(".swift"):
            in_file_path = os.path.join(dir_path, file_name)
            out_file_path = in_file_path.replace(".swift", ".out")
            tests.append(Test(in_file_path, out_file_path))

    return tests

def run_test(test: Test):
    print()
    print(f"Running test: {os.path.basename(test.in_file_path)} ({test.in_file_path})")

    # load input
    with open(test.in_file_path, "rb") as f:
        stdin = f.read()

    # compile
    command = COMPILER_PATH + " " + test.in_file_path
    process = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True, stdin=subprocess.PIPE)

    stdout, stderr = process.communicate(stdin)

    exit_code = process.returncode

    if exit_code != 0:
        print_color_err("Compilation failed")
        print("STDERR:")
        print(stderr.decode('utf-8'))

        print("STDOUT:")
        print(stdout.decode('utf-8'))
        return
    
    if len(stdout) == 0:
        print_color_err("Compilation failed")
        print_color_err("No compiler output")
        # return
    else:
        print(f"Received compiler output {len(stdout)} bytes long.")
    
    # save stdout to file
    compiled_file_path = PROJ_ROOT + "/tmp/" + os.path.basename(test.in_file_path).replace(".swift", ".ic23")
    with open(compiled_file_path, "wb") as f:
        f.write(stdout)
    
    # run interpreter
    print(f"Running interpreter... ({INTERPRETER_PATH})")

    command = [INTERPRETER_PATH, compiled_file_path]

    interpreter_stdout = b""
    interpreter_stderr = b""

    try:
        pipes = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        interpreter_stdout, interpreter_stderr = pipes.communicate()
    except OSError as e:
        print_color_err("Failed to run interpreter (see message below), continuing with empty output for debug purposes...")
        print("    " + str(e))
        # return
    except subprocess.CalledProcessError as e:
        print_color_err("Failed to run interpreter (see message below), continuing with empty output for debug purposes...")
        print("    " + str(e))
        # return

    # read expected output
    with open(test.out_file_path, "r") as f:
        expected_output = f.read()

    # compare output
    if (interpreter_stdout.decode('utf-8') == expected_output):
        print_color_ok("Test passed")
    else:
        print_color_err("Test failed")
        print_color_err("Expected (stdout):")
        print(expected_output)

        print_color_err("Got (stdout):")
        print(interpreter_stdout.decode('utf-8'))

        print_color_err("Got (stderr):")
        print(interpreter_stderr.decode('utf-8'))

        return

    return True
    

def main():
    os.makedirs(TMP_PATH, exist_ok=True)
    tests = get_tests_from_dir(TESTS_DIR)

    failed = 0
    passed = 0

    for test in tests:
        ret = run_test(test)

        if ret:
            passed += 1
        else:
            failed += 1

        print("--------------------------------------------------")

    print()
    print("Tests passed: " + str(passed))

    if failed > 0:
        print_color_err("Tests failed: " + str(failed))


if __name__ == '__main__':
    main()