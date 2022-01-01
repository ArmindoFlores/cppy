import PythonExpressions


def to_literal(value):
    tp = value.get_members()[1]
    if tp == "bool":
        return "true" if value.get_members()[0] else "false"
    if tp == "none":
        return ""
    if tp == "int":
        return str(value.get_members()[0])
    return "?"

class CodeBlock:
    def get_code(self, scope):
        return NotImplemented
    
class CBAssign(CodeBlock):
    def __init__(self, var, value):
        self._var = var
        self._value = value
        
    def get_code(self, scope):
        if isinstance(self._value, PythonExpressions.Literal):
            return f"SCOPE.set_var(\"{self._var.get_members()[0]}\", \"{scope.get_scope_path()}\", cppy::helpers::new_{self._value.get_members()[1]}({to_literal(self._value)}));"
        else:
            return f"SCOPE.set_var(\"{self._var.get_members()[0]}\", \"{scope.get_scope_path()}\", SCOPE.get_var(\"{self._value.get_members()[0]}\", \"{scope.get_scope_path()}\"));"
        
class Scope(CodeBlock):
    def __init__(self, name, parent_ctx=None):
        self.name = name
        self._parent_ctx = parent_ctx
        self._variables = {}
        self._code_blocks = []
        
    def add_cb(self, cb):
        self._code_blocks.append(cb)

    def get_scope_path(self):
        if self._parent_ctx is None:
            return self.name
        return self._parent_ctx.get_scope_path() + "." + self.name
        
    def get_code(self, scope):
        # var_decl_code = "\n".join(
        #     (f"\tPyObject *{var};" for var in self._variables)
        # )
        var_decl_code = ""
        total_code = var_decl_code + (("\n" + self._parent_ctx.get_code()) if self._parent_ctx is not None else "")
        total_code += "\n".join("\n".join(("\t" + line for line in cb.get_code(self).splitlines())) for cb in self._code_blocks)
        total_code = f"\ncppy::PyObjectPtr {self.name}()\n\x7b\n{total_code}\n\x7d"
        return total_code
        
    def has_var(self, name):
        if name in self._variables:
            return True
        if self._parent_ctx is not None:
            return self._parent_ctx.hasvar(name)
        return False
    
    def has_local_var(self, name):
        return name in self._variables
    
    def get_var(self, name):
        if name in self._variables:
            return self._variables[name]
        if self._parent_ctx is not None:
            return self._parent_ctx.getvar(name)
        return None
    
    def add_var(self, name, var):
        if self.has_local_var(name):
            return False
        self._variables[name] = var
        return True