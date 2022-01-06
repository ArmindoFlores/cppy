from typing import List

import antlr4

from . import CodeGeneration
from . import PythonExpressions
from .Python3Visitor import Python3Visitor


class Visitor(Python3Visitor):
    def __init__(self) -> None:
        super().__init__()
        self._global_scope = CodeGeneration.Scope("global")
        self._includes: List[str] = []
        
    def getCode(self) -> str:
        header = "#include \"cppy.h\"\n"
        footer = "\n\nint main()\n{\n\tglobal();\n}\n"
        return header + self._global_scope.get_code(None) + footer
        
    def visitSmall_stmt(self, ctx):
        if ctx.expr_stmt() is not None:
            expr = self.visit(ctx.expr_stmt())
            self._global_scope.add_cb(CodeGeneration.CBStop())
            print(expr)
        else:
            print("[small_stmt] Not implemented")
            return super().visitSmall_stmt(ctx)
        
    def visitAtom_expr(self, ctx):
        expr = self.visit(ctx.atom())
        for trailer in ctx.trailer():
            if trailer.arglist() is not None:
                arglist = self.visitArglist(trailer.arglist())
                expr = PythonExpressions.FunctionCall(expr, arglist)
            else:
                raise NotImplementedError("[atom_expr] Not implemented")
        return expr

    def visitArglist(self, ctx):
        return [self.visit(argument) for argument in ctx.argument()]
    
    def visitAtom(self, ctx):
        if ctx.NAME() is not None:
            return PythonExpressions.Variable(ctx.NAME().getText())
        if ctx.STRING() is not None and len(ctx.STRING()) > 0:
            return PythonExpressions.Literal("".join([s.getText() for s in ctx.STRING()]), "string")
        if ctx.NUMBER() is not None:
            # FIXME: What about floats, complex numbers, and other bases
            return PythonExpressions.Literal(int(ctx.NUMBER().getText()), "int")
        if ctx.FALSE() is not None:
            return PythonExpressions.Literal(False, "bool")
        if ctx.TRUE() is not None:
            return PythonExpressions.Literal(True, "bool")
        if ctx.NONE() is not None:
            return PythonExpressions.Literal(None, "none")
        if ctx.ELLIPSIS() is not None:
            return PythonExpressions.Literal(..., "ellipsis")
        raise NotImplementedError("[atom] Not implemented")
        
    def visitExpr_stmt(self, ctx):
        l = self.visit(ctx.testlist_star_expr(0))
        if len(ctx.testlist_star_expr()) == 2:
            r = self.visit(ctx.testlist_star_expr(1))
        elif len(ctx.testlist_star_expr()) == 1:
            r = None
        else:
            raise NotImplementedError(f"[expr_stmt] Not implemented")
        print("l", l)
        print("r", r)
        if r is not None:
            self._global_scope.add_var(l.get_members()[0], {})
            self._global_scope.add_cb(CodeGeneration.CBAssign(l, r))
        else:
            self._global_scope.add_cb(CodeGeneration.CBName(l))
        
    def visitTestlist_star_expr(self, ctx):
        for child in filter(lambda c: not isinstance(c, antlr4.tree.Tree.TerminalNodeImpl), ctx.getChildren()):
            c = self.visit(child)
            print("test or star")
        return super().visitTestlist_star_expr(ctx)

    def visitTest(self, ctx):
        if ctx.lambdef() is not None:
            raise NotImplementedError("[test] Not implemented")

        else:
            if len(ctx.or_test()) == 1:
                return self.visit(ctx.or_test(0))
            else:
                return PythonExpressions.ConditionalExpr(self.visit(ctx.or_test(0)), self.visit(ctx.test()))
