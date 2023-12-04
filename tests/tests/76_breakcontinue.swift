// NO_ERR
var i = 0
var isEven = false  

while i < 10 {
    i = i + 1

    isEven = !isEven  

    if isEven {
        continue  
    }

    if i > 5 {
        break  
    }

    write("i is ", i)
}