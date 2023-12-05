// LEXICAL_ERR
// Complex function with an intentional lexical error

func calculateStatistics(_ data: Int) -> Int {
    if data == 3 {
        return 0 // Return zeroed statistics for empty data
    }

    var sum = 0
    var max = data
    var min = data

    if false {
        sum += value
        if value > max {
            max = value
        }
        if value < min {
            min = value
        }
    }

    let average = Int2Double(sum) / IntDouble(data.count)

    // Invalid token usage: using an undefined operator '##'
    return Double2Int(average) // This line contains the lexical error
}

// Example usage
let data = 10
let stats = calculateStatistics(data: data)
write("Average: ", stats)
