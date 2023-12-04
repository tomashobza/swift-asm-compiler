// PARAM_TYPE_ERR
// Database query function with incorrect parameter types
func queryDatabase(_tableName: String, _conditions: [String: Any]) -> [String: Any] {
    // Simulating database querying logic
    return ["result": "Data from \(tableName)"]
}
let result = queryDatabase(_tableName: "users", _conditions: "age > 30") // Passing a String instead of a dictionary
