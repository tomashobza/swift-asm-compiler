# Information provided
precedence = {
    "!": 0,
    "*": 1,
    "/": 1,
    "+": 2,
    "-": 2,
    "==": 3,
    "!=": 3,
    "<": 3,
    ">": 3,
    "<=": 3,
    ">=": 3,
    "??": 4,
}

associativity = {
    "!": "none",
    "*": "left",
    "/": "left",
    "+": "left",
    "-": "left",
    "==": "none",
    "!=": "none",
    "<": "none",
    ">": "none",
    "<=": "none",
    ">=": "none",
    "??": "right",
}

# Operators grouped for the table
grouped_operators = ["!", "* /", "+ -", "== != < > <= >=", "??", "i", "(", ")", "$"]


def get_relation(op1_group, op2_group):
    op1s = op1_group.split()
    op2s = op2_group.split()

    # If there are multiple operators in both groups, we compare the first of each.
    # It's an assumption based on the data provided.
    op1 = op1s[0]
    op2 = op2s[0]

    if op1 == "i" or op2 == "(":
        return "<"
    if op2 == "i" or op1 == ")":
        return ">"
    if op1 == "$":
        return "<"
    if op2 == "$":
        return ">"
    if op1 == "(":
        if op2 == ")":
            return "="
        return "<"
    if op2 == ")":
        return ">"

    if precedence[op1] < precedence[op2]:
        return "<"
    elif precedence[op1] > precedence[op2]:
        return ">"
    else:
        if associativity[op1] == "left":
            return ">"
        elif associativity[op1] == "right":
            return "<"
        else:
            return "-"


# Rest of the code remains unchanged


# Generate the table
table = []
header = [""] + grouped_operators
table.append(header)

for op1_group in grouped_operators:
    row = [op1_group]
    for op2_group in grouped_operators:
        if " " in op1_group:  # Multiple operators in the group
            relation = get_relation(
                op1_group.split(" ")[0], op2_group.split(" ")[0]
            )  # Just take the first one as representative
        else:
            relation = get_relation(op1_group, op2_group)
        row.append(relation)
    table.append(row)

# Display the table
for row in table:
    print(",".join(row))
