from src.packages.NativeFunction import NativeFunction
from src.runtime.objects.FalxValue import FalxValue
from abc import ABC

class BoundNativeFunction(NativeFunction, ABC):
    def __init__(self, this: FalxValue):
        super().__init__()
        self.this = this