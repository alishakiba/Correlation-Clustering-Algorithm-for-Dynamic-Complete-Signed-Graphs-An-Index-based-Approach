#include "IndexBasedCorrelationClustering.h"

IndexBasedCorrelationClustering::IndexBasedCorrelationClustering(Graph *g) 
    : NaiveCorrelationClustering(g) 
{
    auto n = this->g->get_n();
    this->naos = new std::unordered_map<unsigned long, NAO*>();
    for(unsigned long i = 0ul; i < n; ++i) {
        this->naos->insert(std::make_pair(i, new NAO(i, this->g)));
    }
}

void IndexBasedCorrelationClustering::reset_naos() {
    for(auto e: *(this->naos)) {
        delete e.second;
    }
    delete this->naos;
    this->reset_g();
    auto n = this->g->get_n();
    this->naos = new std::unordered_map<unsigned long, NAO*>();
    for(unsigned long i = 0ul; i < n; ++i) {
        this->naos->insert(std::make_pair(i, new NAO(i, this->g)));
    }
}

IndexBasedCorrelationClustering::~IndexBasedCorrelationClustering() {
    for(auto e: *(this->naos)) {
        delete e.second;
    }
    delete this->naos;
}

std::vector<unsigned long>* IndexBasedCorrelationClustering::query(double eps) {
    auto n = this->g->get_n();
    // if (this->g_temp != nullptr)
    //     delete this->g_temp;
    // std::cerr << "HELLOEELELELELE*******************" << std::endl;
    this->g_temp = new Graph(this->original_g);
    this->remove_non_agree_edges_and_light_with_index(eps);
    return this->connected_components();
}


void IndexBasedCorrelationClustering::remove_non_agree_edges_and_light_with_index(double eps) {
    // identifying all edges which are in non-eps agreement
    auto non_agree_edges = new std::vector<std::pair<unsigned long, unsigned long>>();
    for(unsigned long i = 0ul; i < this->g->get_n(); ++i) {
        for(auto p: this->naos->at(i)->query(eps)) {
            if (p.first > i)
                non_agree_edges->push_back(std::make_pair(i, p.first));
        }
    }
    // identifying whether vertices are e-light or not
    auto is_light = new std::vector<bool>(this->g->get_n(), false);
    for(unsigned long i = 0ul; i < this->g->get_n(); ++i) {
        if (! this->naos->at(i)->is_heavy(eps))
            (*is_light)[i] = true;
    }
    // removing all non-agree edges
    for(auto edge: *non_agree_edges) {
        this->g_temp->remove_positive_edge(edge.first, edge.second);
    }
    std::cerr << "Index: There are " << non_agree_edges->size() 
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
    for(auto edge: *non_agree_edges) {
        this->g_temp->remove_positive_edge(edge.first, edge.second);
    }
    std::cerr << "Index: There are " << non_agree_edges->size() 
        << " light edges to delete." << std::endl;
    delete is_light;
    delete non_agree_edges;
}

std::map<double, unsigned long>* IndexBasedCorrelationClustering::get_all_eps() {
    auto output = new std::map<double, unsigned long>();
    for(auto nao: *(this->naos)) {
        for(auto el: *(nao.second->get_nao())) {
            (*output)[el.second]++;
        }
    }
    return output;
}
