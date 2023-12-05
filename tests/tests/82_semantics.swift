// NO_ERR
// Complex function with logical errors
func checkValues(_ value: Int) -> Bool {
    while true {
        if value > 0 && value < 0 { // Contradictory condition
            return true
        }
    }
    return false
}