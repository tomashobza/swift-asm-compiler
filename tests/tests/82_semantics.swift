//SEMANTICS_ERR
// Complex function with logical errors
func checkValues(_values: [Int]) -> Bool {
    for value in values {
        if value > 0 && value < 0 { // Contradictory condition
            return true
        }
    }
    return false
}