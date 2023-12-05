from test_runtime import ErrorType, test, nr_failed


def test_parse_fail(code: str):
    test(code, "", expected_return_code=ErrorType.error_parser)


def test_all():
    test("""
    1.2.3
    """, "", expected_return_code=ErrorType.error_lexer)

    test_parse_fail("let a =")
    test_parse_fail("var b")
    test_parse_fail("let a :")
    test_parse_fail("let a : kentus")
    test_parse_fail("let a : Int = ")
    test_parse_fail("var a = : 5")
    test_parse_fail("let")
    test_parse_fail("if true {}")
    test_parse_fail("let a = 5 let b = 4")
    test_parse_fail("let a: Int let b: Double?")
    test_parse_fail("if true {} else {} let a = 5")
    test_parse_fail("if true {let a = 5 let b = 3} else {}")
    test_parse_fail("while true false {}")
    test_parse_fail("while let false {}")
    test_parse_fail("func a")
    test_parse_fail("func a (")
    test_parse_fail("func a () ->")
    test_parse_fail("func a (a: Int)")
    test_parse_fail("func a (a b : )")
    test_parse_fail("func a (a _ : _)")
    test_parse_fail("func a (a _ : kentus)")
    test_parse_fail("a")
    test_parse_fail("ord(")
    test_parse_fail("let a = chr(1)\nsubstring(of: a")
    test_parse_fail("let a = 5\nchr(a")
    test_parse_fail("let a = 5\nchr(a,)")
    test_parse_fail("let func")
    test_parse_fail("func !")
    test_parse_fail("_")
    test("\n", "")
    # this actually caught bugs in another project
    test_parse_fail("kentus blentus")


if __name__ == "__main__":
    test_all()
    exit(nr_failed)
