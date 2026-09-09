class BreakException(Exception):
    """In Falx, we use exceptions/errors to handle break/continues
    so that when we run the interpreting loop we know exactly
    what to do, and the builtin exception error messages handle
    if they are called outside their intended domain
    """
    def __init__(self):
        super().__init__("'break' called outside of loo[")