// SYNTACTIC
// Reference to an out-of-scope variable
func outerFunction() {
    let outerVar = "Outer"
    func innerFunction() {
        print(outerVar) // 'outerVar' is not accessible inside 'innerFunction'
    }
    innerFunction()
}
