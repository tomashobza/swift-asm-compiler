from test_runtime import ErrorType, test, nr_failed


def test_all():
    test("""
    1.2.3
    """, "", expected_return_code=ErrorType.error_lexer)


if __name__ == "__main__":
    test_all()
    exit(nr_failed)
