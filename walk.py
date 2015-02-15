"""
.. walk.py

Walking on graphs.
"""

## Randomness
import random as r


class Walker(object):
    pass


class Walk(object):
    def __init__(self, graph, walkers=None):
        self.graph = graph
        self._set_walkers(walkers)
        self._reset_time()

    def _set_walkers(self, walkers):
        """ Locate the walkers. A walker should know where he is, and a vertex
        should know which walker is on it. """
        ## Create a location mapping
        self._walkers = dict((v, set()) for v in self.graph.V)

        self._locate_walkers(walkers)
        self._notify_walkers()

    def _locate_walkers(self, walkers):
        """ Create a walker map - a dictionary, mapping from vertices to
        walkers. """
        ## Locate the walkers
        for w in walkers:
            self._walkers[1].add(w)

        self._after_locating_hook()

    def _after_locating_hook(self):
        pass

    def _notify_walkers(self):
        for v in self.graph.V:
            for w in self._walkers[v]:
                w.location = v

    def _reset_time(self):
        self.t = 0

    def _next_walking_walkers(self):
        """ Return a set of all the walkers that should move next. These
        walkers will move together. """
        return set()

    def _walker_step(self, walker):
        """ Let *walker* walk. """
        old_location = walker.location
        self._walkers[old_location].remove(walker)
        walker.location = new_location = walker.step(self.graph)
        self._walkers[new_location].add(walker)
        self._after_walker_step_hook(walker, old_location, new_location)

    def _after_walker_step_hook(self, walker, old, new):
        pass

    def step(self):
        """ Let none/some/all of the walkers make their move. """
        walkers = self._next_walking_walkers()
        for w in walkers:
            self._walker_step(w)
        self.t += 1

        self._after_step_hook()

    def _after_step_hook(self):
        pass

    def start(self):
        self._running = True
        self._after_start_hook()
        while self._running:
            self.step()

    def _after_start_hook(self):
        pass

    def stop(self):
        self._running = False


class SimpleWalker(Walker):
    def step(self, graph):
        v = self.location
        N = graph.neighbours(v)
        if not N:
            return v
        return r.choice(N)


class SimpleRandomWalk(Walk):
    """ A simple random walk of one walker. """
    def __init__(self, graph):
        walker = SimpleWalker()
        super(SimpleRandomWalk, self).__init__(graph, [walker])

    def _after_locating_hook(self):
        """ Let the range include the starting point. """
        self._range = set(v for v, ws in self._walkers.viewitems() if ws)

    def _next_walking_walkers(self):
        return set(w for ws in self._walkers.viewvalues() for w in ws)

    def _after_walker_step_hook(self, walker, old, new):
        self._range.add(new)

    def _after_step_hook(self):
        if self._range == self.graph.V:
            self.stop()

    def _after_start_hook(self):
        if self._range == self.graph.V:
            self.stop()

