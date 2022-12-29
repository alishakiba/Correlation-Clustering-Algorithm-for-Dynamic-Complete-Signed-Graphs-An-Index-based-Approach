#include "Graph.h"

unsigned long Graph::deg_positive(unsigned long v) {
    auto neigh_v = this->positive_adjacency->at(v);
    if (neigh_v)
        return neigh_v->size();
    else
        throw std::logic_error("Vertex " + std::to_string(v) + " does not exist.");
}

Graph::Graph() {
    this->positive_adjacency = new std::unordered_map<unsigned long, std::vector<unsigned long>*>();
}

Graph::~Graph() {
    for(auto pair: *(this->positive_adjacency))
        delete pair.second;
}

double Graph::non_agreement(unsigned long u, unsigned long v) {
    try {
        if (this->is_in_neigh_plus(v, u)) {
            auto neigh_u = this->get_neighborhood(u);
            auto neigh_v = this->get_neighborhood(v);
            auto deg_u = neigh_u->size();
            auto deg_v = neigh_v->size();
            std::vector<unsigned long> neigh_intersect((deg_u > deg_v) ? deg_u : deg_v);
            auto it = std::set_intersection(neigh_u->begin(), neigh_u->end(), 
                neigh_v->begin(), neigh_v->end(), neigh_intersect.begin());
            neigh_intersect.resize(it - neigh_intersect.begin());
            return static_cast<double>(deg_u + deg_v - 2 * neigh_intersect.size())
                /(((deg_u > deg_v) ? deg_u : deg_v) + 1);
        } else {
            return INVALID_NON_AGREEMENT;
        }
    }
    catch (std::exception &e) {
        BOOST_LOG_TRIVIAL(fatal) << "For edge {" << u << ", " << v << "}" << e.what();
        throw;
    }
}

void Graph::add_positive_edge(unsigned long u, unsigned long v) {
    if (!this->is_in_neigh_plus(v, u) && u != v) {
        auto * neigh_v = this->positive_adjacency->at(v);
        auto * neigh_u = this->positive_adjacency->at(u);
        this->add_to_sorted_vector(neigh_u, v);
        this->add_to_sorted_vector(neigh_v, u);
        this->m++;
        // BOOST_LOG_TRIVIAL(info) << "Positive edge {" << u << ", " << v << "} added.";
    }
    else 
        // BOOST_LOG_TRIVIAL(info) << "Positive edge {" << u << ", " << v << "} existed.";
        1;
}

void Graph::remove_positive_edge(unsigned long u, unsigned long v) {
    try {
        auto * neigh_v = this->positive_adjacency->at(v);
        auto * neigh_u = this->positive_adjacency->at(u);
        auto it_u_in_v = std::find(neigh_v->begin(), neigh_v->end(), u);
        auto it_v_in_u = std::find(neigh_u->begin(), neigh_u->end(), v);
        // if (it_u_in_v != neigh_v->end()) {
        //     neigh_v->erase(it_u_in_v);
        //     // std::cerr << "Going to delete " << u << std::endl;
        // }
        // if (it_v_in_u != neigh_u->end()) {
        //     neigh_u->erase(it_v_in_u);
        //     // std::cerr << "Going to delete " << v << std::endl;
        // }
        // std::cerr << "Positive edge {" << u << ", " << v << "} removed" << std::endl;
        if (it_u_in_v != neigh_v->end() && it_v_in_u != neigh_u->end()) {
            neigh_v->erase(it_u_in_v);
            neigh_u->erase(it_v_in_u);
            this->m--;
        }
        else {
            // std::cerr << "edge {" << u << "," << v << "}: u in v: "
            //     << (it_u_in_v != neigh_v->end()) << ", v in u: " 
            //     << (it_v_in_u != neigh_u->end()) << std::endl;
            assert(it_u_in_v == neigh_v->end() && it_v_in_u == neigh_u->end());
        }
    }
    catch (std::exception &e) {
        BOOST_LOG_TRIVIAL(fatal) << "For edge {" << u << ", " << v << "}" << e.what();
        throw;
    }
}

unsigned long Graph::where_is_in_neigh_plus(unsigned long v, unsigned long query_vertex) {
    // pre-condition: the neighborhoods are sorted
    auto neigh_v = this->positive_adjacency->at(v);
    auto lower = std::lower_bound(neigh_v->begin(), neigh_v->end(), query_vertex);
    bool found = lower != neigh_v->end() && *lower == query_vertex;
    return found ? lower - neigh_v->begin() : this->m + 1;
}

bool Graph::is_in_neigh_plus(unsigned long v, unsigned long query_vertex) {
    // pre-condition: the neighborhoods are sorted
    try {
        auto neigh_v = this->positive_adjacency->at(v);
        auto lower = std::lower_bound(neigh_v->begin(), neigh_v->end(), query_vertex);
        bool found = lower != neigh_v->end() && *lower == query_vertex;
        return found;
    }
    catch (std::exception &e) {
        BOOST_LOG_TRIVIAL(fatal) << "Query for " << query_vertex << " in neighborhood of " << v << ": " << e.what();
        throw;
    }
}

void Graph::add_to_sorted_vector(std::vector<unsigned long> * vect, unsigned long u) {
    try {
        vect->insert(std::upper_bound(vect->begin(), vect->end(), u), u);
    }
    catch (std::exception &e) {
        BOOST_LOG_TRIVIAL(fatal) << "Add to sorted vector the element " << u << ": " << e.what();
        throw;
    }
}

void Graph::load_from_file(std::string input) {
    // #_of_vertices #_of_edges
    // u_index v_index
    auto failed = false;
    try {
        unsigned long n = 0, m = 0;
        std::ifstream input_file(input, std::ios::in);
        // BOOST_LOG_TRIVIAL(info) << "Open file: " << input;
        if (input_file.is_open()) {
            input_file >> n >> m;
            // BOOST_LOG_TRIVIAL(info) << "vertices: " << n << ", edges: " << m;
            for(unsigned long i = 0u; i < n; ++i) {
                this->positive_adjacency->insert(std::make_pair(i, new std::vector<unsigned long>()));
            }
            unsigned long u = 0u, v = 0u;
            unsigned long i = 0u;
            while(! input_file.eof()) {
                input_file >> u >> v;
                // // BOOST_LOG_TRIVIAL(info) << "Going to add edge {" << u << "," << v << "}";
                this->add_positive_edge(u, v);
                ++i;
            }
            unsigned long sum = 0;
            for(unsigned long i = 0; i < n; ++i) {
                auto neigh = this->get_neighborhood(i);
                assert(std::is_sorted(neigh->begin(), neigh->end()));
                sum += neigh->size();
            }
            assert(sum % 2 == 0);
            // BOOST_LOG_TRIVIAL(info) << "read: " << i << " edges (latest was " << u << "," << v << ")";
            assert(i - 1 == m);
            this->n = n;
            if (sum / 2 != m)
                // BOOST_LOG_TRIVIAL(info) << "Possible duplicate edges in the input file (" << i - 1 << " lines of edges read, " << sum / 2 << " edges added" << ")";
            this->m = sum / 2;
        }
        else
            failed = true;
        input_file.close();
    }
    catch (std::exception &e) {
        BOOST_LOG_TRIVIAL(fatal) << "Error in reading the file: " << input << ": " << e.what();
        throw;
    }
    if (failed)
        throw std::invalid_argument("File error: " + input);
}

unsigned long Graph::add_vertex() {
    try {
        auto new_id = this->positive_adjacency->size();
        assert(this->positive_adjacency->find(new_id) == this->positive_adjacency->end());
        this->positive_adjacency->insert(std::make_pair(new_id, new std::vector<unsigned long>()));
        this->n++;
        return new_id;
    }
    catch (std::exception &e) {
        BOOST_LOG_TRIVIAL(fatal) << "Adding a new vertex failed (n = " << this->get_n() << "): " << e.what();
        throw;
    }
} 

void Graph::remove_vertex(unsigned long v) {
    try {
        auto neigh_v = this->get_neighborhood(v);
        if (neigh_v != nullptr) {
            for(auto u: *neigh_v) {
                this->remove_positive_edge(u, v);
            }
            delete this->positive_adjacency->at(v);
            this->positive_adjacency->at(v) = nullptr;
            this->n--;
        }
        // else, it is already removed
    }
    catch (std::exception &e) {
        BOOST_LOG_TRIVIAL(fatal) << "Removing a vertex failed (v = " << v << "): " << e.what();
        throw;
    }
}

Graph::Graph(const Graph* g) {
    this->m = g->m;
    this->n = g->n;
    this->positive_adjacency = new std::unordered_map<unsigned long, std::vector<unsigned long>*>();
    for(auto pa: *(g->positive_adjacency)) {
        this->positive_adjacency->insert(
            std::make_pair(pa.first, new std::vector<unsigned long>(*(pa.second)))
        );
    }
}
