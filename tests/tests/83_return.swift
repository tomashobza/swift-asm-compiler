// PARAM_TYPE_ERR
// Complex function returning incorrect type
func evaluateCondition(_ condition: Bool) -> Bool {
    if condition {
        return "True Condition" // Returning a String in a Bool function
    }
    return false
}