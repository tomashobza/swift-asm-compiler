// LEXICAL_ERR
// Invalid character and string usage
func invalidSyntaxTest() -> String {
    let invalidChar = "#" // Assuming '#' is not a valid character in IFJ2023
    let invalidString = "This is a test string with an invalid ending: $%" // Assuming '$%' is not valid
    return invalidChar + invalidString
}