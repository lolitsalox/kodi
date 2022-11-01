from enum import StrEnum, IntEnum, auto
from io import BytesIO
from mimetypes import init
from typing import List

class KodObjectType(StrEnum):
    OBJECT = auto()
    CODE = auto()
    FUNCTION = auto()
    NAME = auto()

class KodInstruction(IntEnum):
    LOAD_CONST  = 0
    LOAD_FAST   = auto()
    LOAD_NAME   = auto()
    STORE_NAME  = auto()
    BIN_OP      = auto()
    UN_OP       = auto()
    CJMP        = auto()
    JMP         = auto()
    FUNCTION    = auto()
    RETURN      = auto()
    CALL        = auto()

class KodObject:
    def __init__(self, object_type: KodObjectType) -> None:
        self.type = object_type
        self.ref_count = 0

class KodCodeObject(KodObject):
    def __init__(self, byte_code: bytes) -> None:
        super().__init__(KodObjectType.CODE)
        self.byte_code = byte_code

class KodNameObject(KodObject):
    def __init__(self, name: str, k_object: KodObject):
        super().__init__(KodObjectType.NAME)
        self.name = name
        self.k_object = k_object

class KodScope():
    def __init__(self, parent_scope: "KodScope") -> None:
        self.locals: List[KodNameObject] = []
        self.parent_scope = parent_scope

    def lookup(self, name: str) -> KodNameObject:
        for name_object in self.locals:
            if name_object.name == name:
                return name_object 
        if self.parent_scope is not None:
            return self.parent_scope.lookup(name)
        return None

class KodFunctionObject(KodObject):
    def __init__(self, name: KodNameObject, code_object: KodCodeObject, arg_names: List[str]) -> None:
        super().__init__(KodObjectType.FUNCTION)
        self.name = name
        self.code_object = code_object
        self.arg_names = arg_names
    
    def call(self):
        "Runs self.code_object's byte code instructions"
        pass

# LOAD_NAME
class FunctionCall:
    def __init__(self, function_object: KodFunctionObject, parent_function_call: "FunctionCall", args: List[KodObject]) -> None:
        self.scope = KodScope(parent_function_call.scope)
        self.args = [KodNameObject(arg_name, arg) for arg_name, arg in zip(function_object.arg_names, args)]
        self.function_object = function_object


class CallStack:
    def __init__(self) -> None:
        self.calls: List[FunctionCall] = []
    
    def push(self, function_call: FunctionCall):
        self.calls.append(function_call)
    
    def pop(self) -> FunctionCall:
        return self.calls.pop()


def read(file: BytesIO, n: int) -> int:
    return int.from_bytes(file.read(n))

class Runtime:
    def __init__(self, constants: List[KodObject], main_code_object: KodCodeObject) -> None:
        self.names: List[KodNameObject] = []
        self.constants = constants
        self.main_code_object = main_code_object
        self.stack: List[KodObject] = []
        self.call_stack = CallStack()
    
    def run(self):
        "Runs the main code object of the runtime environment."
        self.run_code_object(self.main_code_object)
    
    def run_code_object(self, code_object: KodCodeObject):
        "Runs a code object in the runtime environment."
        # def f():
        #     pass
        # f()

        # LOAD_CONST 0
        # LOAD_NAME  0
        # FUNCTION
        # STORE_NAME 0
        # LOAD_NAME  0
        # CALL       0
        
        with BytesIO(code_object.byte_code) as byte_code:
            while byte_code.tell() < len(code_object.byte_code):
                op = read(byte_code, 1)

                if op == KodInstruction.LOAD_CONST:
                    self.stack.append(self.constants[read(byte_code, 4)])

                elif op == KodInstruction.LOAD_NAME:
                    self.stack.append(self.names[read(byte_code, 4)])
                
                elif op == KodInstruction.STORE_NAME:
                    self.names[read(byte_code, 4)] = self.stack.pop()

                elif op == KodInstruction.FUNCTION:
                    name: KodNameObject = self.stack.pop()
                    code_object: KodCodeObject = self.stack.pop()
                    function_object = KodFunctionObject(name, code_object)
                    self.stack.append(function_object)
                
                elif op == KodInstruction.CALL:
                    kod_object: KodObject = self.stack.pop()
                    if kod_object.type == KodFunctionObject:
                        function_object: KodFunctionObject = kod_object
                        arg_count = read(byte_code, 2)
                        args = [self.stack.pop(0) for _ in range(arg_count)]
                        function_call = FunctionCall(function_object, args)
                        self.call_stack.push(function_call)
                        self.run_function_call(function_call)
                    else:
                        raise RuntimeError("Not callable")

                else:
                    raise RuntimeError(f"don't know how to handle {op:02X}")
            
    def run_function_call(self, function_call: FunctionCall):
        # def f():
        #     pass
        # f()

        # LOAD_CONST 0
        # LOAD_NAME  0
        # FUNCTION
        # STORE_NAME 0
        # LOAD_NAME  0
        # CALL       0
        function_object = function_call.function_object
        code_object = function_object.code_object
        with BytesIO(code_object.byte_code) as byte_code:
            while byte_code.tell() < len(code_object.byte_code):
                op = read(byte_code, 1)

                if op == KodInstruction.LOAD_CONST:
                    self.stack.append(self.constants[read(byte_code, 4)])

                elif op == KodInstruction.LOAD_NAME:
                    # TODO: go up namespaces in search of the name, currently behaves as LOAD_FAST or something
                    self.stack.append(function_call.locals[read(byte_code, 4)])
                
                elif op == KodInstruction.STORE_NAME:
                    self.locals[read(byte_code, 4)] = self.stack.pop()

                elif op == KodInstruction.FUNCTION:
                    name: KodNameObject = self.stack.pop()
                    code_object: KodCodeObject = self.stack.pop()
                    function_object = KodFunctionObject(name, code_object)
                    self.stack.append(function_object)

                elif op == KodInstruction.CALL:
                    kod_object: KodObject = self.stack.pop()
                    if kod_object.type == KodFunctionObject:
                        function_object: KodFunctionObject = kod_object
                        arg_count = read(byte_code, 2)
                        args = [self.stack.pop(0) for _ in range(arg_count)]
                        function_call = FunctionCall(function_object, args)
                        self.call_stack.push(function_call)
                        self.run_code_object(function_object.code_object)
                    else:
                        raise RuntimeError("Not callable")

                else:
                    raise RuntimeError(f"don't know how to handle {op:02X}")


def main():
    pass
    # Parse kod file to get the constants and main code object
    # runtime = Runtime()

if __name__ == "__main__":
    main()