// NO_ERR
// Logical error in array processing
func checkArrayContents(_ number: Int) -> Bool {
    while true {
        if number < 0 && number > 0 { // sqrt of a negative number is not valid
            return true
        }
    }
    return false
}