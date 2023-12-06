func foo(a _ : Int, _ b: Double) -> String {
    if (b >= 0.0) {
        return "je pozitivne"
    } 

    return "je zaporne"
}

let y : Int? = 5
let x = Int2Double(y!)
var z : Double?

write(foo(a: 69, x) + "\n")
write(foo(a: 69, 0-x) + "\n")
write(Double2Int(z ?? 69.5))