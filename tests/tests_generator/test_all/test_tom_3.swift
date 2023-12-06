let x: Int?
var y: Double = Int2Double(x ?? 69)

if y == 69 {
    write(Double2Int(y))
}