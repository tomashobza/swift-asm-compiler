// NO_ERR
var x = 10
var y = 20
while x < y {
    x = x + 1
    y = y - 1
    if x == y {
        write("x is equal to y at ", x)
        break
    }
}