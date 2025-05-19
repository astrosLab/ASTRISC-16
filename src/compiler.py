import lexer, parser

#def compile(program: str):
#    lexed = lexer.lex(program)

if __name__ == "__main__":
    code = """
    func main() {
        int a = 5;
        if (a == 5) {
            int b = 2;
        }
    }"""
    
    lexed = lexer.lex(code)
    parsed = parser.parse(lexed)

    print(lexed)

