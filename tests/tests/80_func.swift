// NO_ERR
func isMultipleOfThree(_ n: Int) -> Bool {
    return n / 3 * 3 == n
}

func evaluateNumber(_ num: Int) -> String {
    if isMultipleOfThree(num) {
        return "Multiple of three"
    } else {
        return "Not a multiple of three"
    }
}

var i = 1
while i <= 10 {
    let result = evaluateNumber(i)
    write(i, ": ", result, "\n")
    i = i + 1
}