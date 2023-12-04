// NO_ERR
let a : String?

if let a {
    write("chyba, melo se vypsat 'nil'")
} else {
    let b = a
    write(b)
}