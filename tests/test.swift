// NO_ERR
let a = readInt()
let b = readInt()

if let a {
    if (a > 0 && b! > 0) || (a < 0 && (b ?? 0) < 0) {
        write("A a B maji stejne znamenko")
    } else {
        write("A a B maji ruzne znamenko")
    }
} else {
    write("Chyba pri nacitani vstupu")
}