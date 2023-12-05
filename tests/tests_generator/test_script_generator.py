import subprocess
import tempfile
import os
import py_utils as utils

COMP_PATH = "./../../ifjcompiler"
INTERPRETER_PATH = "./../../utils/ic23int"
TEST_DIR = "./test_all"
EXPECTED_DIR = "./expected_out"

def run_compiler_and_interpreter(file_path, expected_output):
    # Check if compiler executable exists
    if not os.path.isfile(COMP_PATH):
        return False, "Compiler \"./../../ifjcompiler\" not found"

       # Start compiler
    try:
        compile_result = subprocess.run(
            f"{COMP_PATH} < {file_path}",
            shell=True,
            capture_output=True,
            text=True,
            timeout=10
        )
        
    except subprocess.TimeoutExpired:
        return False, "Compiler timeout"
    except Exception as e:
        return False, f"Compiler error: {str(e)}"
        
    # Check if compiler output is None
    if compile_result.stdout is None:
        return False, "Compiler returned None"

    # Creating a temp file for the ASM code
    asm_fd, asm_path = tempfile.mkstemp(suffix=".asm")
    try:
        with os.fdopen(asm_fd, "w") as asm_file:
            asm_file.write(compile_result.stdout)

        # Check if interpreter executable exists
        if not os.path.isfile(INTERPRETER_PATH):
            return False, "Interpreter \"ic23int\" not found"

        # Start interpreter
        try:
            interpreter_result = subprocess.run([INTERPRETER_PATH, asm_path], capture_output=True, text=True, timeout=10)
        except subprocess.TimeoutExpired:
            return False, "Interpreter timeout"
        except Exception as e:
            return False, f"Interpreter error: {str(e)}"

        # Check if interpreter output is correct
        if interpreter_result.stdout.strip() != expected_output:
            return False, f"{interpreter_result.stdout}"

        return True, interpreter_result.stdout
    finally:
        os.remove(asm_path)
    
# Test examples
def test_examples():

    swift_files = [f for f in os.listdir(TEST_DIR) if f.endswith('.swift')]
    expected_out = [f for f in os.listdir(EXPECTED_DIR) if f.endswith('.out')]

    for file_name, expected_out_file in zip(swift_files, expected_out):
        file_path = os.path.join(TEST_DIR, file_name)
        base_file_name, _ = os.path.splitext(file_name)
        expected_path = os.path.join(EXPECTED_DIR, expected_out_file)
        with open(file_path, "r") as file:
            source = file.read()
        with open(expected_path, "r") as file:
            expected = file.read()
        result, output = run_compiler_and_interpreter(file_path, expected)
        utils.print_white("-----------------------------------")
        utils.print_white(f"{base_file_name}:")
        print(" ")
        try:
            assert result
            utils.print_green("PASS")
        except AssertionError as e:
            utils.print_red("FAILED")
            utils.print_red(" ")
            utils.print_magenta("Source code:")
            print(source)
            utils.print_magenta("Expected output:")
            print(expected)
            utils.print_magenta("Actual output:")
            print(output)
        print(" ")

if __name__ == "__main__":
    test_examples()
