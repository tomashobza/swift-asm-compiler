func boo () -> Bool {
    if true {
        return true
    } else {
        return false
    }
}

func foo () -> Bool {
    if boo() {
        return true
    } else if boo() {
        if boo() {
            return true
        } else {
            if boo() {
                return true
            }
        }
    } else {
        return false
    }
}