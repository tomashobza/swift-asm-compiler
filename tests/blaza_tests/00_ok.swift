// NO_ERR
var a : String = "hello"
var b : String
var c = "hello"

let d : String = "hello"
let e : String
let f = "hello"

if a {
    write(a)
} else {
    write(a)
}

while a {
    write(a)
}

// Function definition
func fn() {}

func fn2(_ a : String) -> String {
    fn3("a", "b")

    return a
}

func fn3(_ a : String, _ b : String) -> String {
    return a
}

func fn4(aa a : String, bb b : String) -> String {
    return a
}

// Function call
fn()
fn2(a)
fn2("lit")
fn3(a, d)
fn3("lit", "lit")
fn4(aa: a, bb: d)
fn4(aa: "lit", bb: "lit")
write(123, "abc", 123.123)

// Assignment
a = "hello"
let read_str : String? = readString()
let read_int : Int? = readInt()
let read_double : Double? = readDouble()

let a_double : Double = Int2Double(123)
let an_int : Int = Double2Int(123.123)

let len : Int = length(a)
let substr : String? = substring(of :a, startingAt: 0, endingBefore: 1)
let ord_res : Int = ord(a)
let chr_res : String = chr(123)

// Infer type from function call
let infer1 = fn2(a)
