// NO_ERR
func add(_ a: Int, _ b: Int) -> Int {
    return a + b
}

func divide(_ a: Int, _ b: Int) -> Int {
    if b != 0 {
        return a / b
    } else {
        return 0  // Handling division by zero
    }
}

let sumResult = add(10, 20)
let finalResult = divide(sumResult, 3)
write("Final result: ", finalResult)