// VARIABLES_ERR
// Multi-step mathematical operation using undefined variables
func performComplexOperation() {
    let x = 5
    let y = 10
    let stepOneResult = x * y
    let finalResult = stepOneResult + z // 'z' is not defined in this scope
}
