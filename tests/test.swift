func concat(_ x : String, with y : String) -> String {
var x = x + y
return x
}
let a = "ahoj "
var ct : String
ct = concat(a, with: "svete")
write(ct, a)