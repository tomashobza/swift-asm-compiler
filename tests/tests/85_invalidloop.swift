// SYNTACTIC_ERR
// Incorrect syntax in an elaborate function
func processData(_data: [Int]) -> [Int] {
    var result: [Int] = []
    for (var i = 0; i < data.length(); i++) { // Invalid loop syntax and method call
        result.add(data[i] * 2) // Incorrect method for adding to array
    }
    return result
}