// COMPATIBILITY_ERR
let a = 10
let b = 20
let c = "30"

if (a + b) == c {  // Error: Comparing Int result with String
    write("Equal")
} else {
    write("Not equal")
}