#include "NaiveCorrelationClustering.h"

NaiveCorrelationClustering::NaiveCorrelationClustering(Graph *g) {
    this->g = new Graph(g);
    this->original_g = g;
}

void NaiveCorrelationClustering::reset_g() {
    this->g = new Graph(g);
}

NaiveCorrelationClustering::~NaiveCorrelationClustering() {
    delete this->g;
}

std::vector<unsigned long>* NaiveCorrelationClustering::query(double eps) {
    auto n = this->g->get_n();
    if (this->g_temp)
        delete this->g_temp;
    this->g_temp = new Graph(this->original_g);
    this->remove_non_agree_edges_and_light(eps);
    return this->connected_components();
}

std::vector<unsigned long>* NaiveCorrelationClustering::connected_components() {
    // finding connected components
    auto assignment = new std::vector<unsigned long>(this->g->get_n(), -1);
    auto visited = new std::vector<bool>(this->g->get_n(), false);
    unsigned long cluster_id = 1ul;
    auto *stack = new std::stack<unsigned long>();
    unsigned long remaining_vertices = this->g->get_n();
    stack->push(0); // starting vertex is 0
    while(remaining_vertices > 0) {
        // performing a DFS
        while(! stack->empty()) {
            auto vertex = stack->top();
            stack->pop();
            if (! visited->at(vertex)) {
                (*visited)[vertex] = true;
                remaining_vertices--;
                (*assignment)[vertex] = cluster_id;
                for(auto neigh: *(this->g_temp->get_neighborhood(vertex))) {
                    if (! visited->at(neigh)) {
                        stack->push(neigh);
                    }
                }
            }
            // else { it is processed }
        }
        // find the next vertex
        for(unsigned long i = 0ul; i < this->g_temp->get_n(); ++i) {
            if (! (*visited)[i]) {
                stack->push(i);
                break;
            }
        }
        if (! stack->empty()) {
            cluster_id++;
            assert(remaining_vertices > 0);
        }
    }
    delete visited;
    delete stack;
    return assignment;
}

void NaiveCorrelationClustering::remove_non_agree_edges_and_light(double eps) {
    // identifying all edges which are in non-eps agreement
    auto non_agree_edges = new std::vector<std::pair<unsigned long, unsigned long>>();
    auto eps_agree_cnt = new std::vector<unsigned long>(this->g->get_n(), 0);
    auto is_light = new std::vector<bool>(this->g->get_n(), false);
    // counting the # of e-agreement positive edges
    for(unsigned long i = 0ul; i < this->g->get_n(); ++i) {
        for(auto j: *(this->g->get_neighborhood(i))) {
            // as the edges are undirected, you need to consider one side
            if (j > i) {
                if (this->g->non_agreement(i, j) >= eps) {
                    non_agree_edges->push_back(std::make_pair(i, j));
                }
                else {
                    (*eps_agree_cnt)[i]++;
                    (*eps_agree_cnt)[j]++;
                }
            }
        }
    }
    // identifying whether vertices are e-light or not
    for(unsigned long i = 0ul; i < this->g->get_n(); ++i) {
        if (
            (this->g->deg_positive(i) == 0) ||
            (*eps_agree_cnt)[i] < eps * this->g->deg_positive(i)
        ) {
            (*is_light)[i] = true;
        }
    }
    // removing all non-agree edges
    for(auto edge: *non_agree_edges) {
        this->g_temp->remove_positive_edge(edge.first, edge.second);
    }
    std::cerr << "Naive: There are " << non_agree_edges->size() 
        << " non-agree edges to delete." << std::endl;
    // removing light vertices
    delete non_agree_edges;
    non_agree_edges = new std::vector<std::pair<unsigned long, unsigned long>>();
    for(unsigned long i = 0ul; i < this->g->get_n(); ++i) {
        for(auto j: *(this->g->get_neighborhood(i))) {
            if ((*is_light)[i] && (*is_light)[j]) {
                non_agree_edges->push_back(std::make_pair(i,j));
            }
        }
    }
    std::cerr << "Naive: There are " << non_agree_edges->size() 
        << " light edges to delete." << std::endl;
    for(auto edge: *non_agree_edges) {
        this->g_temp->remove_positive_edge(edge.first, edge.second);
    }
    delete is_light;
    delete eps_agree_cnt;
    delete non_agree_edges;
}
