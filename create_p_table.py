import pandas as pd
import sys

# Symbols
symbols = [
    "!",
    "*",
    "/",
    "+",
    "-",
    "==",
    "!=",
    "<",
    ">",
    "<=",
    ">=",
    "??",
    "i",
    "(",
    ")",
    "$",
]

# Initialize the table with empty strings
precedence_table = pd.DataFrame("-", index=symbols, columns=symbols)

# 1) Precedence of operators
precedence_order = [
    ["!"],
    ["*", "/"],
    ["+", "-"],
    ["==", "!=", "<", ">", "<=", ">="],
    ["??"],
]

for i in range(len(precedence_order)):
    for op_i in precedence_order[i]:
        for j in range(len(precedence_order)):
            for op_j in precedence_order[j]:
                if i < j:
                    precedence_table.at[op_i, op_j] = ">"
                    precedence_table.at[op_j, op_i] = "<"
                elif i > j:
                    precedence_table.at[op_i, op_j] = "<"
                    precedence_table.at[op_j, op_i] = ">"

# 2) Associativity
left_associative = ["*", "/", "+", "-"]
right_associative = ["??"]

for op_i in left_associative:
    for op_j in left_associative:
        if op_i != op_j:
            precedence_table.at[op_i, op_j] = ">"
            precedence_table.at[op_j, op_i] = ">"

for op_i in right_associative:
    for op_j in right_associative:
        if op_i != op_j:
            precedence_table.at[op_i, op_j] = "<"
            precedence_table.at[op_j, op_i] = "<"

# 3) Identifiers
for a in symbols:
    precedence_table.at[a, "i"] = "<"
    precedence_table.at["i", a] = ">"


# 5) End of string $
for op_i in symbols:
    if op_i != "$":
        precedence_table.at["$", op_i] = "<"
        precedence_table.at[op_i, "$"] = ">"

# Merging the operators with the same precedence
merged_symbols = ["!", "*/", "+-", "== != < > <= >=", "??", "i", "(", ")", "$"]

symbol_mapping = {
    "!": "!",
    "*": "*/",
    "/": "*/",
    "+": "+-",
    "-": "+-",
    "==": "== != < > <= >=",
    "!=": "== != < > <= >=",
    "<": "== != < > <= >=",
    ">": "== != < > <= >=",
    "<=": "== != < > <= >=",
    ">=": "== != < > <= >=",
    "??": "??",
    "i": "i",
    "(": "(",
    ")": ")",
    "$": "$",
}

merged_precedence_table = pd.DataFrame(
    "-", index=merged_symbols, columns=merged_symbols
)


for row_symbol in symbols:
    for col_symbol in symbols:
        merged_row_symbol = symbol_mapping[row_symbol]
        merged_col_symbol = symbol_mapping[col_symbol]
        current_value = precedence_table.at[row_symbol, col_symbol]

        if current_value:
            merged_precedence_table.at[
                merged_row_symbol, merged_col_symbol
            ] = current_value

# Print the merged table in CSV format
csv_output = merged_precedence_table.to_csv(index=True)

print(csv_output)
