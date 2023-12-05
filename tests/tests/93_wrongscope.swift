// VARIABLES_ERR
// Complex function using variables from an incorrect scope
func outerFunction() {
    let outerVariable = "Accessible"
    if true {
        write(outerVariable) // Correct usage of 'outerVariable'
        write(innerVariable) // Incorrect usage, 'innerVariable' is not defined in this scope
    }
    let innerVariable = "Not Accessible"
    innerFunction()
}