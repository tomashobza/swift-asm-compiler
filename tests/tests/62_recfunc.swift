// NO_ERR
func recursiveSum(_ n: Int) -> Int {
    if n <= 0 {
        return 0
    } else {
        return n + recursiveSum(n)  // Logical error: should be recursiveSum(n - 1)
    }
}

let total = recursiveSum(5)