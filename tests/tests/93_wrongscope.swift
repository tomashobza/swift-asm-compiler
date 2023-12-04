// VARIABLES_ERR
// Complex function using variables from an incorrect scope
func outerFunction() {
    let outerVariable = "Accessible"
    func innerFunction() {
        print(outerVariable) // Correct usage of 'outerVariable'
        print(innerVariable) // Incorrect usage, 'innerVariable' is not defined in this scope
    }
    let innerVariable = "Not Accessible"
    innerFunction()
}