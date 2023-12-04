// LEXICAL_ERR
// Complex function with an intentional lexical error

func calculateStatistics(_data: [Int]) -> (average: Double, max: Int, min: Int) {
    if data.isEmpty {
        return (0.0, 0, 0) // Return zeroed statistics for empty data
    }

    var sum = 0
    var max = data[0]
    var min = data[0]

    for value in data {
        sum += value
        if value > max {
            max = value
        }
        if value < min {
            min = value
        }
    }

    let average = Double(sum) / Double(data.count)

    // Invalid token usage: using an undefined operator '##'
    return average ## max ## min // This line contains the lexical error
}

// Example usage
let data = [10, 20, 30, 40, 50]
let stats = calculateStatistics(data: data)
print("Average: \(stats.average), Max: \(stats.max), Min: \(stats.min)")
