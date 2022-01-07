from . import PythonExpressions


class CodeBlock:
    def get_code(self, scope):
        return NotImplemented
    
class CBAssign(CodeBlock):
    def __init__(self, var, value):
        self._var = var
        self._value = value
        
    def get_code(self, scope):
        return f"SCOPE.set_var(\"{self._var.get_members()[0]}\", \"{scope.get_scope_path()}\", {self._value.get_code(scope)});"

class CBName(CodeBlock):
    def __init__(self, var):
        self._var = var

    def get_code(self, scope):
        return self._var.get_code(scope) + ";"

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
        total_code = f"\ncppy::PyObjectPtr {self.name}()\n\x7b\n{total_code}\n\treturn cppy::helpers::new_none();\n\x7d"
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

class CBIf(CodeBlock):
    def __init__(self, if_condition, if_body, elifs_conditions, elifs_bodies, else_body):
        self._if_cond = if_condition
        self._if_body = if_body
        self._elifs_conds = elifs_conditions
        self._elifs_bodies = elifs_bodies
        self._else_body = else_body

    def get_code(self, scope):
        if_text = f"if (cppy::helpers::cbool({self._if_cond.get_code(scope)})) " + "{\n"
        if_text += "\n".join("\n".join(("\t" + line for line in cb.get_code(scope).splitlines())) for cb in self._if_body)
        if_text += "\n}\n"

        for i in range(len(self._elifs_conds)):
            if_text += f"else if (cppy::helpers::cbool({self._elifs_conds[i].get_code(scope)})) " + "{\n"
            if_text += "\n".join("\n".join(("\t" + line for line in cb.get_code(scope).splitlines())) for cb in self._elifs_bodies[i])
            if_text += "\n}\n"

        if self._else_body is not None:
            if_text += "else {\n"
            if_text += "\n".join("\n".join(("\t" + line for line in cb.get_code(scope).splitlines())) for cb in self._else_body)
            if_text += "\n}\n"

        return if_text