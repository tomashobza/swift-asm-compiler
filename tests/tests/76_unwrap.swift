// NO_ERR
func fibonacci(_ n: Int) -> Int {
    if n <= 1 {
        return n
    } else {
        return fibonacci(n - 1) + fibonacci(n - 2)
    }
}

let n = readInt()
if let n {
    let y = readInt()!
    write("Fibonacciho cislo: ", fibonacci(n))
} else {
    write("Chyba pri nacitani vstupu")
}