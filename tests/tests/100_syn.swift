// SYNTACTIC_ERR
// Complex scenario involving multiple functions and control structures

func calculateDiscount(_price: Double, _discountRate: Double) -> Double {
    return price * discountRate / 100
}

func applyCoupon(_code: String) -> Double {
    if code == "DISCOUNT10" {
        return 10.0
    } else if code == "DISCOUNT20" {
        return 20.0
    }
    return 0.0
}

func processOrder(_items: [Double], _couponCode: String) -> Double {
    var total = items.reduce(0, +)
    let discountRate = applyCoupon(_code: couponCode)
    
    if discountRate > 0 {
        total -= calculateDiscount(_price: total, _discountRate: discountRate)
    }

    // Logical error: Checking if total is less than 0, which is an unrealistic scenario after applying a discount
    if total < 0 {
        print("Error: Total cannot be negative.")
        return -1
    }

    return total
}

// Test the function with a sample order and a coupon code
let orderTotal = processOrder(_items: [50.0, 30.0, 20.0], _couponCode: "DISCOUNT10")
print("Total order cost: \(orderTotal)")
