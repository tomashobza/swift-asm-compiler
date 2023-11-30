// FUNCTIONS_ERR
func process(_ a: Int) -> Int {
    return a * 2
}

func process(_ a: String) -> Int {
    return length(a)
}

let result = process(5)  // Error: Ambiguous function call