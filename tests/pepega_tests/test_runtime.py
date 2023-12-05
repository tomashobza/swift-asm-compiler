import traceback
import subprocess
from enum import Enum

COMPILER_PATH = "./../../ifjcompiler"
INTERPRETER_PATH = "./ic23int"

# gtfo python
true = True
false = False

nr_failed = 0


class ErrorType(Enum):
    # successful compilaton
    success = 0

    # chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního
    # lexému
    error_lexer = 1

    # chyba v programu v rámci syntaktické analýzy (chybná syntaxe programu
    # chybějící hlavička atp.)
    error_parser = 2

    # sémantická chyba v programu – nedefinovaná funkce, redefinice proměnné
    error_ident = 3

    # sémantická chyba v programu – špatný počet/typ parametrů u volání funkce
    # či špatný typ návratové hodnoty z funkce
    error_call = 4

    # sémantická chyba v programu – použití nedefinované nebo neinicializované
    # proměnné
    error_undefined_var = 5

    # sémantická chyba v programu – chybějící/přebývající výraz v příkazu
    # návratu z funkce
    error_invalid_return = 6

    # sémantická chyba typové kompatibility v aritmetických řetězcových a
    # relačních výrazech
    error_type = 7

    # sémantická chyba odvození typu – typ proměnné nebo parametru není uveden
    # a nelze odvodit od použitého výrazu
    error_type_inference = 8

    # ostatní sémantické chyby
    error_other_semantic = 9

    # interní chyba překladače tj. neovlivněná vstupním programem (např. chyba
    # alokace paměti atd.)
    error_internal = 99


def print_red_bold(input: str):
    print("\033[31;1m" + input + "\033[0m")


def print_red(input: str):
    print("\033[31m" + input + "\033[0m")


def print_yellow(input: str):
    print("\033[33m" + input + "\033[0m")


def print_error(error_message: str):
    print("")
    print_red_bold(error_message)
    print_yellow(traceback.format_stack()[1].split('\n')[0])


def test(input_code: str, expected_output: str, program_stdin: str = "",
         expected_return_code: ErrorType = ErrorType.success,
         expect_runtime_error: bool = false):
    global nr_failed
    # running compiler

    compiler_output = subprocess.run(
        COMPILER_PATH, input=input_code.encode(), capture_output=true)

    if compiler_output.returncode == -11:
        print_error("Your compiler segfaulted")
        nr_failed += 1
        return

    compiler_return_code = ErrorType(compiler_output.returncode)

    compiler_stdout = compiler_output.stdout
    compiler_stderr = compiler_output.stderr

    def print_compiler_info():
        print_red("compiler stdin:")
        print(input_code)
        print_red("compiler stdout:")
        print(compiler_stdout.decode())
        print_red("compiler stderr:")
        print(compiler_stderr.decode())
        print_red(
            f"compiler returned error code {compiler_return_code} ({compiler_output.returncode})")

    # check expected compiler return code
    if expected_return_code != compiler_return_code:
        print_error("Error: compiler return code does not match expected value")
        print_compiler_info()
        nr_failed += 1
        return

    if compiler_return_code != ErrorType.success:
        return

    with open("generated_code", "w") as f:
        f.write(compiler_stdout.decode())

    # running interpreter

    interp_output = subprocess.run([
        INTERPRETER_PATH, "generated_code"], input=program_stdin.encode(), capture_output=true)

    interp_stdout = interp_output.stdout
    interp_stderr = interp_output.stderr

    def print_inpterp_info():
        print_red("expected output:")
        print(expected_output)
        print_red("interpreter stdout:")
        print(interp_stdout.decode())
        print_red("interp_stderr:")
        print(interp_stderr.decode())

    if expect_runtime_error and interp_output.returncode == 0:
        print_error("Error: interpreter was expected to fail, but succeeded")
        print_compiler_info()
        print_inpterp_info()
        nr_failed += 1
        return

    if not expect_runtime_error and interp_output.returncode != 0:
        print_error(
            f"Error: interpreter failed with exit code {interp_output.returncode}")
        print_compiler_info()
        print_inpterp_info()
        nr_failed += 1
        return

    if str(expected_output) != interp_stdout.decode():
        print_error(
            "Error: Interpreter output differs from expected output")
        print_compiler_info()
        print_inpterp_info()
        nr_failed += 1


def print_exit():
    print("")
    print("All tests passed." if nr_failed ==
          0 else f"{nr_failed} tests failed")
    exit(0 if nr_failed == 0 else 1)
