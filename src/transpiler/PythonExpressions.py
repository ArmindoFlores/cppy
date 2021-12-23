class Expr:    
    def get_members(self):
        return NotImplemented
    
class Literal(Expr):
    def __init__(self, value, tp):
        self._value = value
        self._type = tp
        
    def __repr__(self):
        return f"Literal<{self._value}>"
    
    def get_members(self):
        return (self._value, self._type)
    
class Variable(Expr):
    def __init__(self, name):
        self._name = name
        
    def __repr__(self):
        return f"Variable<{self._name}>"    
        
    def get_members(self):
        return (self._name,)
    
class FunctionCall(Expr):
    def __init__(self, what, args):
        self._what = what
        self._args = args
        
    def __repr__(self):
        return f"{self._what}{self._args}"
    
    def get_members(self):
        return (self._what, *self._args)

class ConditionalExpr(Expr):
    def __init__(self, true, false):
        self._true = true
        self._false = false
        
    def __repr__(self):
        return f"{self._true} if [?] else {self._false}"
        
    def get_members(self):
        return self._true, self._false

# class 
        