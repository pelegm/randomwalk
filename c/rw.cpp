#include <algorithm>
#include <iostream>
#include <map>
#include <math.h>
#include <random>

// Random number generator
std::random_device randomDevice;
std::default_random_engine randomEngine(randomDevice());
std::uniform_real_distribution<double> P(0.0, 1.0);
typedef std::uniform_int_distribution<int> randint;

class Graph {
    private:
        int** A;

    public:
        int n;

        // Constructor
        Graph(int n=1) {
            this->n = n;
            A = new int*[n];
            for (int i = 0; i < n; i++) {
                A[i] = new int[n];
                for (int j = 0; j < n; j++)
                    A[i][j] = 0;
            }
        }

        void addEdge(int u, int v) {
            if (u >= 1 && u <= n && v >= 1 && v <= n) {
                A[u-1][v-1]++;
                if (u != v) {
                    A[v-1][u-1]++;
                }
            }
        }

        void removeEdge(int u, int v) {
            /**
             * Remove at most 1 edge from u to v.
             */
            if (u >= 1 && u <= n && v >= 1 && v <= n) {
                A[u-1][v-1]--;
                if (u != v) {
                    A[v-1][u-1]--;
                }
            }
        }

        int removeEdges(int u, int v) {
            /**
             * Remove all edges from u to v, return the number of removed
             * edges.
             */
            int r = 0;
            if (u >= 1 && u <= n && v >= 1 && v <= n) {
                r = A[u-1][v-1];
                A[u-1][v-1] = 0;
                if (u != v) {
                    A[v-1][u-1] = 0;
                }
            }
            return r;
        }

        int countEdges(int u, int v) {
            /**
             * Return the number of edges from u to v.
             */
            if (u >= 1 && u <= n && v >= 1 && v <= n) {
                return A[u-1][v-1];
            } else {
                return 0;
            }
        }

        int m() {
            /**
             * Return the number of edges in the graph.
             */
            int _m = 0;
            for (int u = 1; u <= n; u++) {
                for (int v = u; v <= n; v++) {
                    _m += A[u-1][v-1];
                }
            }
            return _m;
        }

        int simpleEdges() {
            /**
             * Return the number of simple edges in the graph.
             */
            int _m = 0;
            for (int u = 1; u < n; u++) {
                for (int v = u+1; v <= n; v++) {
                    _m += A[u-1][v-1] > 0;
                }
            }
            return _m;
        }

        std::map<int, int> N(int u) {
            /**
             * Return the edge-neighbourhood of u as a map
             */
            std::map<int, int> _N;
            if (u >= 1 && u <= n) {
                int c = 0;
                for (int v = 1; v <= n; v++) {
                    for (int j = 0; j < A[u-1][v-1]; j++) {
                        c++;
                        _N[c] = v;
                    }
                }
            }
            return _N;
        }

//        ~Graph() {
//            for (int i = 0; i < n; i++) {
//                delete[] A[i];
//            }
//            delete[] A;
//        }
};

Graph completeGraph(int n) {
    Graph G(n);
    for (int u = 1; u < n; u++) {
        for (int v = u+1; v <= n; v++) {
            G.addEdge(u, v);
        }
    }
    return G;
};

Graph cycleGraph(int n) {
    Graph G(n);
    for (int u = 1; u < n; u++) {
        G.addEdge(u, u+1);
    }
    G.addEdge(n, 1);
    return G;
}

Graph randomGraph(int n, double p) {
    Graph G(n);
    for (int u = 1; u < n; u++) {
        for (int v = u+1; v <= n; v++) {
            if (P(randomEngine) < p) {
                G.addEdge(u, v);
            }
        }
    }
    return G;
};

class Walk {
    public:
        Graph G;
        int loc;
        int t;
        Graph trace;
        std::map<int, int> visits;

        // Constructor
        Walk(Graph G, int start = 1) : G(G), loc(start) {
            this->t = 0;
            this->trace = Graph(G.n);

            for (int i = 0; i < this->G.n; i++) {
                this->visits[i] = 0;
            }
            this->visits[this->loc] = 1;
        }

        int step() {
            /**
             * Make a random step, return the new location.
             */
            std::map<int,int> N = this->G.N(this->loc);
            int Ns = N.size();
            if (Ns == 0) {
                throw 1;
            }
            int to = N[randint(1, Ns)(randomEngine)];
            this->makeStep(to);
            return to;
        }

        bool walk(int L) {
            /**
             * Walk L steps. Return true if nothing bad has happened.
             */
            for (int t = 0; t < L; t++) {
                try {
                    this->step();
                }
                catch (int e) {
                    std::cout << "Random walk is stuck." << std::endl;
                    return false;
                }
            }
            return true;
        }

        int walkTillCover(int c=1) {
            /**
             * Walk until G is covered c times. Return the final time.
             */
            while (this->hasCovered() < c) {
                this->step();
            }
            return this->t;
        }

        int hasCovered() {
            /**
             * Return the cover number of the walk on G.
             */
            int c = this->t + 1;
            for (int u = 1; u <= this->G.n; u++) {
                c = std::min(c, this->visits[u]);
            }
            return c;
        }

    private:
        void makeStep(int to) {
            /**
             * Make a step to *to*.
             */
            this->trace.addEdge(this->loc, to);

            // Uncomment this to be a burner walker
            this->G.removeEdge(this->loc, to);

            //std::cout << "Stepped from " << this->loc << " to " << to <<
            //   std::endl;

            this->loc = to;
            this->t++;
            this->visits[to]++;
        }
};

int main() {
    int n = 500;

    Graph G = completeGraph(n);
    //Graph G = randomGraph(n, log(n)/n);
    //Graph G = cycleGraph(n);

    std::cout << G.n << " vertices" << std::endl;
    std::cout << G.m() << " edges" << std::endl;

    Walk X = Walk(G);
    std::cout << X.trace.m() << " edges in the trace" << std::endl;
    X.walk(pow(n, 2));
    //int ct = X.walkTillCover(1);
    std::cout << X.trace.m() << " edges in the trace" << std::endl;
    std::cout << "Cover number is " << X.hasCovered() << std::endl;
    //std::cout << "Cover time is " << ct << std::endl;

    int edges_left = G.m();
    std::cout << "Edges left: " << edges_left << std::endl;
}
