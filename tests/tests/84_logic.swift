// NO_ERR
// Function with complex but logically flawed conditions
func validateArray(_ number: Int) -> Bool {
    while (number < 10 && number > 0) || number == 0-1 {
        if number > 0 && number == 0-number { // Logically impossible condition
            write("Invalid number detected")
            return true
        }
    }
    return false
}