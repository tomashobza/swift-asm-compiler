// NO_ERR
func multiplyAndAdd(_ a: Int, _ b: Int, _ c: Int) -> Int {
    return (a * b) + c
}

func processNumber(_ n: Int) -> Int {
    if n < 0 {
        return -1
    } else {
        return multiplyAndAdd(n, n + 1, n - 1)
    }
}

let result = processNumber(5)
write("Processed number result: ", result)
