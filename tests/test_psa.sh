echo "========= SCENARIO 1 ========="
bin/ifjcompiler_debug <tests/test1.in
echo "=============================="
echo "SHOULD BE: \033[1;32mOK\033[0m"
echo "=============================="

echo "========= SCENARIO 2 ========="
bin/ifjcompiler_debug <tests/test2.in
echo "=============================="
echo "SHOULD BE: \033[1;32mOK\033[0m"
echo "=============================="

echo "========= SCENARIO 3 ========="
bin/ifjcompiler_debug <tests/test3.in
echo "=============================="
echo "SHOULD BE: \033[1;32mOK\033[0m"
echo "==============================\n\n\n\n"

echo "========= SCENARIO 4 ========="
bin/ifjcompiler_debug <tests/test_wrong1.in
echo "=============================="
echo "SHOULD BE: \033[1;31mWRONG\033[0m"
echo "==============================\n\n\n\n"

echo "========= SCENARIO 5 ========="
bin/ifjcompiler_debug <tests/test_wrong2.in
echo "=============================="
echo "SHOULD BE: \033[1;31mWRONG\033[0m"
echo "==============================\n\n\n\n"

echo "========= SCENARIO 6 ========="
bin/ifjcompiler_debug <tests/test_wrong3.in
echo "=============================="
echo "SHOULD BE: \033[1;31mWRONG\033[0m"
echo "==============================\n\n\n\n"

echo "========= SCENARIO 7 ========="
bin/ifjcompiler_debug <tests/test_wrong4.in
echo "=============================="
echo "SHOULD BE: \033[1;31mWRONG\033[0m"
echo "==============================\n\n\n\n"
