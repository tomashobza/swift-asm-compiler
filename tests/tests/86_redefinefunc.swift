// FUNCTIONS_ERR
// Redefinition of functions and incorrect usage
func compute(_a: Int, _b: Int) -> Int {
    return a + b
}
func compute(_a: Int, _b: Int, _c: Int) -> Int { // Redefining existing function
    return a * b * c
}
func executeComplex() {
    let value = compute(x: 5, y: 10) // Incorrect parameter names
}