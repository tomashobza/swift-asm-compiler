import subprocess
import tempfile
import os

def run_compiler_and_interpreter(source_code, expected_output):
    # Creating a temp file for the source code
    src_fd, src_path = tempfile.mkstemp(suffix='.srcSwift')
    try:
        with os.fdopen(src_fd, 'w') as src_file:
            src_file.write(source_code)

        # Check if compiler executable exists
        if not os.path.isfile('./ifjcompiler'):
            return False, "Compiler './ifjcompiler' not found"

        # Start compiler
        try:
             compile_result = subprocess.run(
                ['./ifjcompiler'],
                input=source_code,
                text=True,
                timeout=10
            )
        except subprocess.TimeoutExpired:
            return False, "Compiler timeout"
        except Exception as e:
            return False, f"Compiler error: {str(e)}"

        # Creating a temp file for the ASM code
        asm_fd, asm_path = tempfile.mkstemp(suffix='.asm')
        try:
            with os.fdopen(asm_fd, 'w') as asm_file:
                asm_file.write(compile_result.stdout)

            # Check if interpreter executable exists
            if not os.path.isfile('./ifj20vm'):
                return False, "Interpreter './ifj20vm' not found"

            # Start interpreter
            try:
                interpreter_result = subprocess.run(['./ifj2023', asm_path], capture_output=True, text=True, timeout=40)
            except subprocess.TimeoutExpired:
                return False, "Interpreter timeout"
            except Exception as e:
                return False, f"Interpreter error: {str(e)}"

            # Check if interpreter output is correct
            if interpreter_result.stdout.strip() != expected_output:
                return False, f"Expected output {expected_output}, but got {interpreter_result.stdout.strip()}"

            return True, interpreter_result.stdout.strip()
        finally:
            os.remove(asm_path)
    finally:
        os.remove(src_path)
    
# Test examples
def test_examples():
    examples = [
        {"source": "write('Hello, world!')", "expected": "Hello, world!"},
        # Other examples...
    ]

    for example in examples:
        result, output = run_compiler_and_interpreter(example["source"], example["expected"])
        assert result, f"Test failed: expected output {example['expected']}, but got {output}"

if __name__ == "__main__":
    test_examples()
