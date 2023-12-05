// NO_ERR
var i = 0
var isEven = false  

while i < 10 {
    i = i + 1

    isEven = !isEven  

    if isEven {
        write("a")
    }

    if i > 5 {
        write("b")
    }

    write("i is ", i)
}