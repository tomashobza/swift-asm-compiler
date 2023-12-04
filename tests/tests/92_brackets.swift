// SYNTACTIC_ERR
// Complex function with mismatched and missing syntax elements
func complexAnalysis(_data: [String]) -> Int {
    var count = 0
    for item in data {
        if (item.startsWith("A") { // Missing closing parenthesis
            count += 1
        } else if item.endsWith("Z" { // Missing closing parenthesis
            count -= 1;
        } // Missing closing brace for 'else if' and 'for' block
    }
    return count
}