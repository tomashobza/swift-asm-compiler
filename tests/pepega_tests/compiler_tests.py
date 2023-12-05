import tokenizer_tests
import parser_tests
import rec_parser_tests
import backend_tests

from test_runtime import ErrorType, test, print_exit


def test_all():
    # 2lazy2sort
    test("""
    let a = 1 + 1
    write(a)
    """, "2")

    test("""
    let a = nil ?? 5
    write(a)
    """, "5")

    test("""
    let a: Int? = nil
    let b = a ?? 4
    write(nil, b)
    """, "4")

    test("""
    let a = 5
    let b = "ahoj"
    let c = a + b
    """, "", expected_return_code=ErrorType.error_type)

    test("""
    var a = 0
    while a < 5 {
        write(a)
        a = a + 1
    }
    """, "01234")

    test("""
    var a = readInt()
    var b = readInt()
    if a! == b! {
        write("eq")
    } else {
        write("ne")
    }
    """, "eq", program_stdin="10\n10")

    test("""
    let a = 5\r\nwrite(a)
    """, "5")

    test("""
    let input = "ifj projekt je moc hard"
    let mock = mock(input)
    write(mock)

    func is_even(_ i: Int) -> Bool {
        let int = i / 2 * 2 // integer division

        let i_d = Int2Double(i)
        let d = i_d / 2.0 * 2.0
        let double = Double2Int(d)

        return int == double
    }

    func mock_char(_ s: String) -> String {
        var c = ord(s)

        if c >= 65 { if c <= 90 { // big -> small
            c = c + 32
            let new = chr(c)
            return new
        } else {} } else {}

        if c >= 97 { if c <= 122 { // small -> big
            c = c - 32
            let new = chr(c)
            return new
        } else {} } else {}

        return s
    }

    // you know what it is
    func mock(_ s: String) -> String {
        let length = length(s)
        var idx = 0
        var output = ""

        while idx < length {
            let end = idx + 1
            let char_opt = substring(of: s, startingAt: idx, endingBefore: end)
            let char = char_opt!

            let is_even = is_even(idx)

            if is_even {
                let mock_char = mock_char(char)
                output = output + mock_char
            } else {
                output = output + char
            }

            idx = idx + 1
        }

        return output
    }
    """, "IfJ PrOjEkT Je mOc hArD")

    # primo ze zadani, proto to nedava smysl
    test("""
    func concat(_ x : String, with y : String) -> String {
        let x = x + y
        return x + " " + y
    }

    let a = "ahoj "
    var ct : String
    ct = concat(a, with: "svete")
    write(ct, a)
    """, "ahoj svete sveteahoj ")

    test("""
        let cycles = 50

        func is_inside(_ real: Double, _ imag: Double) -> Bool {
            var x = real
            var y = imag

            var cycle = 0

            while cycle < cycles {
                let tmp = x
                x = x * x - y * y + real
                y = 2 * tmp * y + imag

                if x * x + y * y > 4 {
                    return false
                } else {}

                cycle = cycle + 1
            }

            return true
        }

        let size = readInt()
        if let size {
            let size_d = Int2Double(size)
            var y = 0 - size
            while y < size {
                var x = 0 - 2 * size

                while x < size {
                    var real = Int2Double(x)
                    var imag = Int2Double(y)
                    real = real / size_d
                    imag = imag / size_d

                    let is_inside = is_inside(real, imag)

                    if is_inside { write() }
                    else { write() }

                    x = x + 1
                }

                write()
                y = y + 1
            }
        } else {
            write("Cannot parse input")
        }

    """, "", program_stdin="10")

    from backend_tests import hexfloat
    test("""
    // Program 1: Vypocet faktorialu (iterativne)
    /* Hlavni telo programu */
    write("Zadejte cislo pro vypocet faktorialu\\n")
    let a : Int? = readInt()
    if let a {
        if (a < 0) {write("Faktorial nelze spocitat\\n")
        } else {
            var a = Int2Double(a)
            var vysl : Double = 1
            while (a > 0) {
                vysl = vysl * a
                a = a - 1
            }
            write("Vysledek je: ", vysl, "\\n")
        }
    } else {
        write("Chyba pri nacitani celeho cisla!\\n")
    }
    """, f"Zadejte cislo pro vypocet faktorialu\nVysledek je: {hexfloat(120.0)}\n",
         program_stdin="5")

    test("""
    // Program 2: Vypocet faktorialu (rekurzivne)
    // Hlavni telo programu
    write("Zadejte cislo pro vypocet faktorialu: ")
    let inp = readInt()

    // pomocna funkce pro dekrementaci celeho cisla o zadane cislo
    func decrement(of n: Int, by m: Int) -> Int {
        return n - m
    }

    // Definice funkce pro vypocet hodnoty faktorialu
    func factorial(_ n : Int) -> Int {
        var result : Int?
        if (n < 2) {
            result = 1
        } else {
            let decremented_n = decrement(of: n, by: 1)
            let temp_result = factorial(decremented_n)
            result = n * temp_result
        }
        return result!
    }
    // pokracovani hlavniho tela programu
    if let inp {
        if (inp < 0) { // Pokracovani hlavniho tela programu
            write("Faktorial nelze spocitat!")
        } else {
            let vysl = factorial(inp)
            write("Vysledek je: ", vysl)
        }
    } else {
        write("Chyba pri nacitani celeho cisla!")
    }
    """, "Zadejte cislo pro vypocet faktorialu: Vysledek je: 120", program_stdin="5\n")

    test("""
    /* Program 3: Prace s retezci a vestavenymi funkcemi */
    var str1 = "Toto je nejaky text v programu jazyka IFJ23"
    let str2 = str1 + ", ktery jeste trochu obohatime"
    write(str1, "\\n", str2, "\\n")
    let i = length(str1)
    write("Pozice retezce \\"text\\" v str2: ", i, "\\n")
    write("Zadejte serazenou posloupnost vsech malych pismen a-h, ")

    let newInput = readString()
    if let newInput {
        str1 = newInput
        while (str1 != "abcdefgh") {
            write("Spatne zadana posloupnost, zkuste znovu:\\n")
            let tmp = readString()
            str1 = tmp ?? ""
        }
    } else {}
    """, """Toto je nejaky text v programu jazyka IFJ23
Toto je nejaky text v programu jazyka IFJ23, ktery jeste trochu obohatime
Pozice retezce "text" v str2: 43
Zadejte serazenou posloupnost vsech malych pismen a-h, Spatne zadana posloupnost, zkuste znovu:
""", program_stdin="kentus\nabcdefgh")

    # bruh
    test("", "")


if __name__ == "__main__":
    test_all()
    tokenizer_tests.test_all()
    parser_tests.test_all()
    rec_parser_tests.test_all()
    backend_tests.test_all()
    print_exit()
