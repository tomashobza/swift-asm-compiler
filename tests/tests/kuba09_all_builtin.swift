// readString() -> String?
// readInt() -> Int?
// readDouble() -> Double?
// write ( term1 , term2 , …, term𝑛 )
// Int2Double(_ term ∶ Int) -> Double
// Double2Int(_ term ∶ Double) -> Int
// length(_ 𝑠 : String) -> Int
// substring(of 𝑠 : String, startingAt 𝑖 : Int, endingBefore 𝑗 : Int) -> String?
// ord(_ 𝑐 : String) -> Int
// chr(_ 𝑖 : Int) -> String

var x = readString()
var y = readInt()
var z = readDouble()
write(x, y, z)
write("\n")

var y1 = readInt()!
var a = Int2Double(y1)
var z1 = readDouble()!
var b = Double2Int(z1)
var x1 = readString()!
var c = length(x1)
write(a, b, c)
write("\n")

var k = substring(of: x1, startingAt: y1, endingBefore: b)
write(k)
write("\n")

var d = ord(x1)
write(d)
write("\n")

var e = chr(y1)
write(e)
