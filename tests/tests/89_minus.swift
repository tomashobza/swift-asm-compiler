// SEMANTICS_ERR
// Logical error in array processing
func checkArrayContents(_numbers: [Int]) -> Bool {
    for number in numbers {
        if number < 0 && sqrt(number) > 0 { // sqrt of a negative number is not valid
            return true
        }
    }
    return false
}