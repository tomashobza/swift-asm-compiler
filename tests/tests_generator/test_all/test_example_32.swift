func fibonacci(_ n: Int) -> Int {
    if n <= 1 {
        return n 
    } else { 
        return fibonacci(n - 1) + fibonacci(n - 2) } 
        }
        
        write(fibonacci(5))
