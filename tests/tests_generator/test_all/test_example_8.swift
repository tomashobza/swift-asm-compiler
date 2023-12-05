var a = 0 
var b = 1 
for i in 1...5 { var temp = a 
a = b 
b = temp + b } 
write(a)