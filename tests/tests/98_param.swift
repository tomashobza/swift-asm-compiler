// NO_ERR
// Database query function with incorrect parameter types
func queryDatabase(_ tableName: String, _ conditions: String) -> String {
    // Simulating database querying logic
    return "Data from" + tableName
}
let result = queryDatabase(tableName: "users", conditions: "age > 30") // Passing a String instead of a dictionary
