// TYPE_ERR
// Ambiguous type in a complex operation
func ambiguousOperation() {
    let unknown = nil // Type cannot be inferred
    let result = unknown * unknown // Operation on ambiguous types
    print(result)
}