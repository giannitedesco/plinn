"""
plinn. Partition Like It's 1999
"""

from .__version__ import \
    __title__, \
    __description__, \
    __url__, \
    __author__, \
    __author_email__, \
    __copyright__, \
    __license__, \
    __version__  # noqa

from ._partition import partition

__all__ = (
    'partition',
)
