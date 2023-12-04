#!/bin/bash

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

    # Předá obsah souboru do 'ifj-compiler'
    "$COMPILER_PATH" < "$OUTPUT_FILE"

    # Získá návratový kód 'ifj-compiler'
    RET_VAL=$?

    # Kontroluje návratový kód
    if [ $RET_VAL -eq 2 ] || [ $RET_VAL -eq 1 ]; then
        echo "Návratový kód ifj-compiler je 1 nebo 2, ukončení smyčky."
        break
    fi
done
