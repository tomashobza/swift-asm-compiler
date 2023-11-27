// NO_ERR
var str = "opakovani"
while true {
    write(str)
    str = str + " opakovani"
    if str == "opakovani opakovani opakovani opakovani opakovani opakovani opakovani opakovani opakovani opakovani" {
        break
    }
}
write(str)