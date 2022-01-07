from typing import List

import antlr4

from . import CodeGeneration
from . import PythonExpressions
from .Python3Visitor import Python3Visitor


class Visitor(Python3Visitor):
    def __init__(self) -> None:
        super().__init__()
        self._global_scope = CodeGeneration.Scope("global")
        self._current_scope = self._global_scope
        self._includes: List[str] = []
        
    def getCode(self) -> str:
        header = "#include \"cppy.h\"\n#include <iostream>\n"
        footer = "\n\nint main()\n{\n\tTB.push(\"<module>\");\n\ttry {\n\t\tglobal();\n\t}\n\tcatch(cppy::globals::traceback_exception &e) {\n\t\tstd::cout << e.what() << std::flush;\n\t}\n}\n"
        return header + self._current_scope.get_code(None) + footer
        
    def visitSmall_stmt(self, ctx):
        if ctx.expr_stmt() is not None:
            expr = self.visit(ctx.expr_stmt())
        else:
            print("[small_stmt] Not implemented")
            return super().visitSmall_stmt(ctx)

    def visitIf_stmt(self, ctx):
        if_condition = self.visit(ctx.test(0))
        last_block = len(self._current_scope._code_blocks)
        self.visit(ctx.suite(0))
        if_body = self._current_scope._code_blocks[last_block:]
        self._current_scope._code_blocks = self._current_scope._code_blocks[:last_block]

        elifs_conditions = []
        elifs_bodies = []
        for i in range(len(ctx.ELIF())):
            elifs_conditions.append(self.visit(ctx.test(i+1)))
            self.visit(ctx.suite(i))
            elifs_bodies.append(self._current_scope._code_blocks[last_block:])
            self._current_scope._code_blocks = self._current_scope._code_blocks[:last_block]

        else_body = None
        if ctx.ELSE():
            self.visit(ctx.suite()[-1])
            else_body = self._current_scope._code_blocks[last_block:]
            self._current_scope._code_blocks = self._current_scope._code_blocks[:last_block]

        if_stmt = CodeGeneration.CBIf(if_condition, if_body, elifs_conditions, elifs_bodies, else_body)
        self._current_scope.add_cb(if_stmt)
        
    def visitAtom_expr(self, ctx):
        expr = self.visit(ctx.atom())
        for trailer in ctx.trailer():
            if trailer.arglist() is not None:
                arglist = self.visitArglist(trailer.arglist())
                expr = PythonExpressions.FunctionCall(expr, arglist)
            else:
                raise NotImplementedError("[atom_expr] Not implemented")
        return expr

    def visitArith_expr(self, ctx):
        v = [self.visit(term) for term in ctx.term()]

        if len(v) == 1:
            return v[0]

        operations = ctx.ADD() + ctx.MINUS()
        operations.sort(key=lambda node: node.getPayload().tokenIndex)

        expr = None
        for i, operation in enumerate(operations):
            if expr is None:
                expr = PythonExpressions.OpExpr(v[i], v[i+1], operation.getText())
            else:
                expr = PythonExpressions.OpExpr(expr, v[i+1], operation.getText())

        return expr

    def visitTerm(self, ctx):
        v = [self.visit(factor) for factor in ctx.factor()]

        if len(v) == 1:
            return v[0]

        operations = ctx.STAR() + ctx.DIV() + ctx.IDIV() + ctx.AT() + ctx.MOD()
        operations.sort(key=lambda node: node.getPayload().tokenIndex)

        expr = None
        for i, operation in enumerate(operations):
            if expr is None:
                expr = PythonExpressions.OpExpr(v[i], v[i+1], operation.getText())
            else:
                expr = PythonExpressions.OpExpr(expr, v[i+1], operation.getText())

        return expr

    def visitPower(self, ctx):
        if ctx.factor() is None:
            return self.visit(ctx.atom_expr())
        return PythonExpressions.OpExpr(self.visit(ctx.atom_expr()), self.visit(ctx.factor()), "**")

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
        if ctx.OPEN_PAREN() is not None and ctx.testlist_comp() is not None:
            return self.visit(ctx.testlist_comp())
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
            self._current_scope.add_var(l.get_members()[0], {})
            self._current_scope.add_cb(CodeGeneration.CBAssign(l, r))
        else:
            self._current_scope.add_cb(CodeGeneration.CBName(l))
        
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
