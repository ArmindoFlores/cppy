class Expr:    
    def get_members(self):
        return NotImplemented
    
class Literal(Expr):
    def __init__(self, value, tp):
        self._value = value
        self._type = tp

    def to_literal(self):
        if self._type == "bool":
            return "true" if self._value else "false"
        if self._type == "none":
            return ""
        if self._type == "int":
            return str(self._value)
        if self._type == "string":
            return self._value
        return "?"
        
    def __repr__(self):
        return f"Literal<{self._value}>"
    
    def get_members(self):
        return (self._value, self._type)

    def get_code(self, scope):
        return f"cppy::helpers::new_{self._type}({self.to_literal()})"
    
class Variable(Expr):
    def __init__(self, name):
        self._name = name
        
    def __repr__(self):
        return f"Variable<{self._name}>"    
        
    def get_members(self):
        return (self._name,)

    def get_code(self, scope):
        return f"SCOPE.get_var(\"{self._name}\", \"{scope.get_scope_path()}\")"
    
class FunctionCall(Expr):
    def __init__(self, what, args):
        self._what = what
        self._args = args
        
    def __repr__(self):
        return f"{self._what}({self._args})"
    
    def get_members(self):
        return (self._what, *self._args)

    def get_code(self, scope):
        args = "{" + ", ".join((arg.get_code(scope) for arg in self._args)) + "}"
        return f"cppy::helpers::call({self._what.get_code(scope)}, cppy::FunctionArguments({args}))"

class ConditionalExpr(Expr):
    def __init__(self, true, false):
        self._true = true
        self._false = false
        
    def __repr__(self):
        return f"{self._true} if [?] else {self._false}"
        
    def get_members(self):
        return self._true, self._false

class OpExpr(Expr):
    OP = {
        "+": "add",
        "-": "sub",
        "*": "mul",
        "/": "truediv",
        "//": "divmod"
    }

    def __init__(self, left, right, op):
        self._left = left
        self._right = right
        self._op = op

    def __repr__(self):
        return f"{self._left} {self._op} {self._right}"

    def get_members(self):
        return self._op, self._left, self._right

    def get_code(self, scope):
        
        return f"cppy::helpers::{self.OP[self._op]}({self._left.get_code(scope)}, {self._right.get_code(scope)})"