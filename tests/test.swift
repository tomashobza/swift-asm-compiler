func boo () -> Bool {
    if true {
        return true
    } else {
        return false
    }
}

func foo () -> String {
    while boo() {
        return "test"
    }
    return "test"
}