// NO_ERR
func sum(a: Int, b: Int) -> Int {
    return a + b
}

let x = readInt()
let y = readInt()
var total = 0

if let x, let y {
    for i in 0..<x {
        total = sum(a: total, b: y)
    }
    write("Vysledny soucet: ", total)
} else {
    write("Chyba pri nacitani vstupu")
}