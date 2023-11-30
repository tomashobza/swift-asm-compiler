// PARAM_TYPE_ERR
func multiply(_ a: Int, _ b: Int) -> Int {
    return a * b
}

func subtract(_ a: Int, _ b: Int) -> Int {
    return a - b
}

let result = multiply(subtract(10, 5), "string")  // Error: Second parameter should be Int, not String