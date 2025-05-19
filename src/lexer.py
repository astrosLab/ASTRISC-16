KEYWORDS = {
    "func": "FUNC",
    "return": "RETURN",
    "halt": "HALT",
    "int": "TYPE_INT",
    "if": "IF",
    "else": "ELSE",
    "while": "WHILE",
    "for": "FOR",
    "break": "BREAK",
    "continue": "CONTINUE"
}

SYMBOLS = {
    "(": "LPAREN",
    ")": "RPAREN",
    "{": "LBRACE",
    "}": "RBRACE",
    ";": "SEMICOLON",
    "=": "ASSIGN",
    "+": "ADD",
    "-": "SUB",
    "*": "MUL",
    "/": "DIV",
    "%": "MOD",  
    "==": "EQUAL",
    "!=": "NOT_EQUAL",
    ">": "GREATER",
    ">=": "GREATER_EQUAL",
    "<": "LESSER",
    "<=": "LESSER_EQUAL",
    "&": "AND",
    "|": "OR",
    "^": "XOR",
    "~": "NOT",
    "<<": "SH_LEFT",
    ">>": "SH_RIGHT",
}

def lex(code: str):
    tokens = []

    i = 0

    while i < len(code):
        char = code[i]

        if char.isspace():
            i += 1
            continue

        if char.isalpha() or char == "_":
            start = i
            while i < len(code) and (code[i].isalnum() or code[i] == "_"):
                i += 1
            word = code[start:i]
            token_type = KEYWORDS.get(word, "IDENT")
            tokens.append((token_type, word))
            continue

        if char.isdigit():
            start = i
            while i < len(code) and code[i].isdigit():
                i += 1
            number = code[start:i]
            tokens.append(("NUMBER", number))
            continue

        if i + 1 < len(code) and code[i:i+2] in SYMBOLS:
            tokens.append((SYMBOLS[code[i:i+2]], code[i:i+2]))
            i += 2
            continue
        if char in SYMBOLS:
            tokens.append((SYMBOLS[char], char))
            i += 1
            continue       

        print(f"Lexer error: Unexpected character '{char}'")
        i += 1

    return tokens
