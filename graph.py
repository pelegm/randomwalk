"""
.. graph.py

The graph class.
"""

class Hypergraph(object):
    """ A :class:`Graph` is a tuple of a vertex set, which is always the set of
    integers from 1 to n for some n>0, and an edge set, which is partial to the
    set of subsets 2 of the vertex set. """
    def __init__(self, n, edges):
        self.n = n
        self._set_vertices(n)
        self._set_edges(edges)

    def _set_vertices(self, n):
        self.V = frozenset(range(1, n + 1))

    def _is_legal_edge(self, e):
        return e <= self.V

    def _set_edges(self, edges):
        for e in edges:
            if not self._is_legal_edge(e):
                raise ValueError("{e} is an illegal edge.".format(e=e))
        self.E = edges

    def incident_edges(self, vertex):
        if not vertex in self.V:
            raise ValueError("{v} is not in {H}.".format(v=vertex, H=self))
        return [e for e in self.E if vertex in e]

    def neighbours(self, vertex):
        if not vertex in self.V:
            raise ValueError("{v} is not in {H}.".format(v=vertex, H=self))
        return [v for e in self.incident_edges(vertex) for v in e
                if v != vertex]


class Graph(Hypergraph):
    """ A :class:`Graph` is a tuple of a vertex set, which is always the set of
    integers from 1 to n for some n>0, and an edge set, which is partial to the
    set of subsets of cardinality 2 of the vertex set. """
    def _is_legal_edge(self, e):
        return super(Graph, self)._is_legal_edge(e) and len(e) == 2

