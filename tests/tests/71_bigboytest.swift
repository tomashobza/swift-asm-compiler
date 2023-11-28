// NO_ERR
func multiply(_ a: Int, _ b: Int) -> Int {
    return a * b
}

func divide(_ a: Int, _ b: Int) -> Int {
    if b != 0 {
        return a / b
    } else {
        return -1  // Safeguard for division by zero
    }
}

func isEven(_ n: Int) -> Bool {
    return n % 2 == 0
}

func factorial(_ n: Int) -> Int {
    if n <= 1 {
        return 1
    } else {
        return n * factorial(n - 1)
    }
}

func findMax(_ a: Int, _ b: Int) -> Int {
    if a > b {
        return a
    } else {
        return b
    }
}

func calculateExpression(_ a: Int, _ b: Int, _ c: Int) -> Int {
    let part1 = multiply(a, b)
    let part2 = factorial(c)
    let result = divide(part1, part2)
    return result
}

// Main execution starts here
let num1 = 6
let num2 = 3
let num3 = 2

let resultMultiply = multiply(num1, num2)
write("Multiplication result: ", resultMultiply)

let resultDivide = divide(resultMultiply, num3)
write("Division result: ", resultDivide)

if isEven(resultDivide) {
    write("Result is even")
} else {
    write("Result is odd")
}

let factorialResult = factorial(num3)
write("Factorial result: ", factorialResult)

let maxNumber = findMax(num1, num2)
write("Maximum number is: ", maxNumber)

let finalResult = calculateExpression(num1, num2, num3)
write("Final calculated result: ", finalResult)

// Conditional checks
if finalResult > 0 {
    write("Final result is positive")
} else {
    write("Final result is non-positive")
}
