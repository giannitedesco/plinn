from typing import List, Any, Callable, Iterable, TypeVar, Tuple

T = TypeVar('T')

def partition(pred: Callable[[T], bool], it: Iterable[T]) \
        -> Tuple[List[T], List[T]]: ...
