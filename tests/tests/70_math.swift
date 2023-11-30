// NO_ERR
let x = 15
let y = 5
let z = 20

if (x > 10 && y <= 5) || (z == 0 && x + y > z) {
    let result = x * y + z
    write("Complex expression result: ", result)
} else {
    write("Condition not met")
}