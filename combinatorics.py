"""
.. combinatorics.py

Combinatorics tools.
"""

## Tools
import itertools as it


def cache(function):
    def wrapper(*args):
        try:
            return function.cache[args]
        except AttributeError:
            result = function(*args)
            function.cache = {}
            function.cache[args] = result
        except KeyError:
            result = function(*args)
            function.cache[args] = result
        return result
    return wrapper


@cache
def _int_choose(n, k):
    if k > n:
        return 0

    if n == 0:
        return 1

    if k > n / 2:
        return choose(n, n - k)

    if k == 0:
        return 1

    if k == 1:
        return n

    ## Avoid maximum recursion error
    for _n in range(1, n):
        for _k in range(1, k + 1):
            _int_choose(_n, _k)

    return _int_choose(n  - 1, k - 1) + _int_choose(n - 1, k)


def _set_choose(s, k):
    return frozenset(frozenset(c) for c in it.combinations(s, k))


def choose(n, k):
    """ If n is an integer, return the binomial coefficient (n choose k). If n
    is a set, return the set of all subsets of n of cardinality k. """
    if isinstance(n, frozenset):
        return _set_choose(n, k)

    if min(n, k) < 0 or (not isinstance(n, int)) or (not isinstance(k, int)):
        raise ValueError("choose is defined on nonnegative integers only.")

    return _int_choose(n, k)

