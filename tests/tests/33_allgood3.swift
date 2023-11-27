// NO_ERR
func power(base: Int, exponent: Int) -> Int {
    var result = 1
    return result
}

let base = readInt()
let exponent = readInt()

if let base, let exponent {
    write("Vysledek: ", base * exponent)
} else {
    write("Chyba pri nacitani vstupu")
}