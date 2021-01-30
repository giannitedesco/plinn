from typing import Callable, Iterable, List, Tuple, FrozenSet
from pathlib import Path
from itertools import tee, filterfalse
from functools import reduce
from timeit import timeit

from plinn import partition as plinn  # noqa


def _stdlib(pred, it):
    a, b = tee(it)
    return filterfalse(pred, a), filter(pred, b)


def stdlib(pred: Callable[[int], bool], it: Iterable[int]) \
        -> Tuple[List[int], List[int]]:
    fs, ts = _stdlib(pred, it)
    return list(fs), list(ts)


def trevor(pred: Callable[[int], bool], it: Iterable[int]) \
        -> Tuple[List[int], List[int]]:
    ret: Tuple[List[int], List[int]] = ([], [])
    for item in it:
        ret[pred(item)].append(item)
    return ret


def mariy(pred, it):
    def f(x, y):
        return x[pred(y)].append(y) or x  # noqa
    return reduce(f, it, ([], []))


def byers(pred, it):
    ts = []
    fs = []
    for item in it:
        if pred(item):
            ts.append(item)
        else:
            fs.append(item)
    return fs, ts


def scara(pred, it):
    ts: List[str] = []
    fs: List[str] = []
    t = ts.append
    f = fs.append
    for item in it:
        if pred(item):
            t(item)
        else:
            f(item)
    return fs, ts


def scara2(pred, it):
    ts: List[str] = []
    fs: List[str] = []
    t = ts.append
    f = fs.append
    for item in it:
        (t if pred(item) else f)(item)
    return fs, ts


haystack: Tuple[str, ...]
needles: FrozenSet[str]


def pred(x):
    return x in needles


def main():
    global haystack, needles
    haystack = tuple(Path('/usr/share/dict/words').read_text().splitlines())
    needles = frozenset((word for word in haystack if word.startswith('s')))
    funcs = (
        'stdlib',
        'mariy',
        'trevor',
        'byers',
        'scara',
        'scara2',
        'plinn',
    )
    for func_name in funcs:
        print(func_name, timeit(
            f'{func_name}(pred, haystack)',
            setup=f'from __main__ import haystack, pred, {func_name}',
            number=32,
        ))


if __name__ == '__main__':
    main()
