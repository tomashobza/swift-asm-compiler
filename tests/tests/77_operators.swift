// NO_ERR
func checkConditions(_ a: Int, _ b: Int) -> Bool {
    if (a < b) && !(a == 0) {
        if b != 10 {
            return true
        } else {
            return false
        }
    } else if a > b || a == 0-1 {
        return true
    } else {
        return false
    }
}

let result = checkConditions(5, 10)
write("Result of conditions: ", result)