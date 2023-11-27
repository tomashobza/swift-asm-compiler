// NO_ERR
func power(base: Int, exponent: Int) -> Int {
    var result = 1
    for _ in 0..<exponent {
        result *= base
    }
    return result
}

let base = readInt()
let exponent = readInt()

if let base, let exponent {
    write("Vysledek: ", power(base: base, exponent: exponent))
} else {
    write("Chyba pri nacitani vstupu")
}