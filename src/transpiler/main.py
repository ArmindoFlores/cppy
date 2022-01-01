from antlr4 import CommonTokenStream, FileStream
from antlr4.tree.Trees import Trees

from Python3Lexer import Python3Lexer
from Python3Parser import Python3Parser
from Visitor import Visitor


def main(filename, outfile):
    i = FileStream(filename)
    lexer = Python3Lexer(i)
    stream = CommonTokenStream(lexer)
    parser = Python3Parser(stream)
    tree = parser.file_input()
    
    print(Trees.toStringTree(tree, None, parser))
    
    visitor = Visitor()
    visitor.visit(tree)

    
    with open(outfile, "w") as f:
        f.write(visitor.getCode())
    

if __name__ == "__main__":
    main("../../tests/test.py", "../../tests/test.cpp")
