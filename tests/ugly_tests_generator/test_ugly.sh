#!/bin/bash
#set -e

# Kontrola, zda byly zadány dva parametry
if [ "$#" -ne 3 ]; then
    echo "Použití: $0 [cesta k gen] [cesta k ifj-compiler]"
    exit 1
fi

GEN_PATH="$1"
COMPILER_PATH="$2"
OUTPUT_FILE="$3"

while true; do
    # Spustí 'gen' a uloží výstup do souboru
    "$GEN_PATH" 2>/dev/null > "$OUTPUT_FILE"
    cat "$OUTPUT_FILE"

    # Předá obsah souboru do 'ifj-compiler'
    "$COMPILER_PATH" >/dev/null 2>/dev/null < "$OUTPUT_FILE"

    # Získá návratový kód 'ifj-compiler'
    RET_VAL=$?

    # Kontroluje návratový kód
   if { [[ $RET_VAL -lt 3 ]] || [[ $RET_VAL -gt 9 ]]; } && [[ $RET_VAL -ne 99 ]] && [[ $RET_VAL -ne 0 ]]; then
        echo "Návratový kód ifj-compiler: $RET_VAL"
        break
    fi

    printf "\033[42m\033[1;37m You shall PASS! \033[0m\n"
done
