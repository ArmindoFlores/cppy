import argparse
import os
import shlex
import subprocess

from antlr4 import CommonTokenStream, FileStream
from antlr4.tree.Trees import Trees

from .Python3Lexer import Python3Lexer
from .Python3Parser import Python3Parser
from .Visitor import Visitor


def parse_args():
    parser = argparse.ArgumentParser(description="A Python to C++ transpiler")
    parser.add_argument("option", choices=["transpile", "compile", "run"], help="What action to take")
    parser.add_argument("-o", "--output", help="Executable name", default="main")
    parser.add_argument("-m", "--mode", choices=["debug", "release"], default="release", help="Compilation mode")
    parser.add_argument("-lm", "--link-mode", choices=["static", "shared"], default="static", help="Linking mode (for the cppy library)")
    parser.add_argument("-c", "--compiler", default="g++", help="Compiler name. Defaults to 'g++'")
    parser.add_argument("-O", "--compiler-options", help="Options to be directly passed to the compiler")
    parser.add_argument("filename", help="File to be compiled")
    return parser.parse_args()

def main(args):
    i = FileStream(args.filename)
    lexer = Python3Lexer(i)
    stream = CommonTokenStream(lexer)
    parser = Python3Parser(stream)
    tree = parser.file_input()
    
    print(Trees.toStringTree(tree, None, parser))
    
    visitor = Visitor()
    visitor.visit(tree)
    
    with open(args.output+".cpp", "w") as f:
        f.write(visitor.getCode())

    if args.option == "transpile":
        return

    try:
        base_args = [args.compiler, "-std=c++17", "-ggdb" if args.mode == "debug" else "-O3"]
        if args.mode == "release":
            base_args.append("-s")
        base_args.extend(["-o", args.output])
        base_args.append(args.output+".cpp")
        base_args.extend(["-I../src/python", "-l:libcppy.a" if args.link_mode == "static" else "-lcppy"])
        if args.compiler_options:
            base_args.extend(shlex.split(args.compiler_options))
        print(base_args)
        process = subprocess.run(base_args)
        if process.returncode != 0:
            print("Failed to build")
            return
    finally:
        os.remove(args.output+".cpp")

    if args.option == "compile":
        return

    subprocess.run(["./"+args.output])
    

if __name__ == "__main__":
    main(parse_args())
