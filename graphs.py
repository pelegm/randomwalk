"""
.. graphs.py

Specific graphs.
"""

## Framework
import graph

## Tools
import combinatorics as c


def _V(n):
    return frozenset(range(1, n + 1))


def complete_graph(n):
    return graph.Graph(n, c.choose(_V(n), 2))
