// SYNTACTIC_ERR
// Advanced file processing with a syntax error
func readFileLines(_filePath: String) -> [String] {
    var lines = [String]()
    // Simulating file reading loop with a syntax error
    while let line = readLine(filePath) { // Incorrect loop syntax
        lines.append(line)
    }
    return lines
}
