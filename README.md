# plinn: Partition Like It's 1999

<div align="center">
  <img src="https://img.shields.io/pypi/v/plinn?label=pypi" alt="PyPI version">
</div>

## The partition function

This module implements a function called "partition."

It is an optimized version of the following python function:

```python
def partition(pred, it):
    f = []
    t = []
    for item in it:
        (t if pred(item) else f).append(item)
    return f, t
```

## Benchmarks
Here are some benchmarks on my i7-6600U laptop with python 3.9.0 comparing
against several pure-python variants of the same function:

| Variant  | Time for 32 iterations (seconds) |
|----------|------------------------|
|stdlib    | 3.2970685160253197     |
|mariy     | 3.5445828151423484     |
|trevor    | 2.6459597239736468     |
|byers     | 2.3595181389246136     |
|scara     | 2.2327406019903720     |
|scara2    | 2.1951448819600046     |
|plinn     | 1.5203454510774463     |
