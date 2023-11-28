// NO_ERR
func sum(_ a: Int, _ b: Int) -> Int {
    return a + b
}

let x = readInt()
let y = readInt()
var total = 0

if let y {
    while true {
        total = sum(total, y)
    }
    write("Vysledny soucet: ", total)
} else {
    write("Chyba pri nacitani vstupu")
}