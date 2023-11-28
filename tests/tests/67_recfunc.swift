// NO_ERR
func factorial(_ n: Int) -> Int {
    if n < 0 {
        return -1  // Error handling for negative input
    } else {
        return n * factorial(n - 1)
    }
}

let result = factorial(5)
write("Factorial result: ", result)