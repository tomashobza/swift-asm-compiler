// SYNTACTIC_ERR
// Complex function with mismatched and missing syntax elements
func complexAnalysis(_ data: String, _ item: Int) -> Int {
    var count = 0
    while true {
        if (item { // Missing closing parenthesis
            count += 1
        } else if item { // Missing closing parenthesis
            count -= 1;
        } // Missing closing brace for 'else if' and 'for' block
    }
    return count
}