// SEMANTICS_ERR
// Function with complex but logically flawed conditions
func validateArray(_numbers: [Int]) -> Bool {
    for number in numbers {
        if number > 0 && number == 0-number { // Logically impossible condition
            print("Invalid number detected")
            return true
        }
    }
    return false
}