class ContinueException(Exception):
    def __init__(self):
        super().__init__("'continue' called outside of loop or function")