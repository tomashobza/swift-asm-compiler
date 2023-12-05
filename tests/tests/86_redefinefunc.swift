// FUNCTIONS_ERR
// Redefinition of functions and incorrect usage
func compute(_ a: Int, _ b: Int) -> Int {
    return a + b
}
func compute(_ a: Int, _ b: Int, _ c: Int) -> Int { // Redefining existing function
    return a * b * c
}
func executeComplex() {
    let value = compute(x: 5, y: 10) // Incorrect parameter names
}