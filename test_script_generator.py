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
            if not os.path.isfile('./utils/ic23int'):
                return False, "Interpreter './utils/ic23int' not found"

            # Start interpreter
            try:
                interpreter_result = subprocess.run(['./utils/ic23int', asm_path], capture_output=True, text=True, timeout=40)
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
        {"source": "write('Hello, world!')\nwrite('Hello, world!')", "expected": "Hello, world!Hello, world!"},
        {"source": "write('Hello, world!')\nwrite('Hello, world!')\nwrite('Hello, world!')", "expected": "Hello, world!Hello, world!Hello, world!"},
        {"source": "write('Hello, world!')\nwrite('Hello, world!')\nwrite('Hello, world!')\nwrite('Hello, world!')", "expected": "Hello, world!Hello, world!Hello, world!Hello, world!"},
        {"source": "var a = 5 \n write(a)", "expected": "5"},
        {"source": "var a = 5 \n var b = 10 \n write(a + b)", "expected": "15"},
        {"source": "var a = 0 \n var b = 1 \n for i in 1...5 { var temp = a \n a = b \n b = temp + b } \n write(a)", "expected": "5"},  # Fibonacci
        {"source": "var number = 10 \n if number > 5 { write('OK') }", "expected": "OK"},
        {"source": "var number = 4 \n if number > 5 { write('OK') } else { write('NOT OK') }", "expected": "NOT OK"},
        {"source": "write('1') \n write('2') \n write('3') \n write('4')", "expected": "1234"}
    ]
    

    for example in examples:
        result, output = run_compiler_and_interpreter(example["source"], example["expected"])
        assert result, f"Test failed: expected output {example['expected']}, but got {output}"

if __name__ == "__main__":
    test_examples()
