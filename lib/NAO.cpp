#include "NAO.h"

NAO::NAO(unsigned long v, Graph *g) {
    this->v = v;
    this->deg_v = g->deg_positive(v);
    this->nao = new std::vector<std::pair<unsigned long, double>>();
    auto neigh_v = g->get_neighborhood(v);
    for(auto u: *neigh_v) {
        this->nao->push_back(
            std::make_pair(u, g->non_agreement(u,v))
        );
    }
    std::sort(this->nao->begin(), this->nao->end(), 
        [](std::pair<unsigned long, double> a, std::pair<unsigned long, double> b) {
            return sort_order(a, b);
        }
    );
}

NAO::~NAO() {
    delete this->nao;
}

std::vector<std::pair<unsigned long, double>> NAO::query(double eps) {
    auto it = std::lower_bound(this->nao->begin(), this->nao->end(), eps, 
        [](std::pair<unsigned long, double> a, double b) {
            return a.second < b;
        }
    );
    std::vector<std::pair<unsigned long, double>> result(it, this->nao->end());
    return result;
}

bool NAO::is_heavy(double eps) {
    unsigned long eps_th = static_cast<unsigned long>(std::ceil(eps * this->deg_v));
    if (this->deg_v > 0) 
        eps_th--;
    else 
        return false; 
    //< No need to add 1, since v is not in the nao list.
    //< In Lemma 2, 1 is added since v was assumed to be there
    //< Moreover, since we start indexing at 0 here, we used floor
    
    // std::cerr << "Node " << this->v << ": "
    //     << "eps_th: " << eps_th << ", deg: " << this->deg_v
    //     << ", eps_th_val: " << ((eps_th < this->deg_v) ? this->nao->at(eps_th).second : -1.0)
    //     // << ", eps_th_val - 1: " << ((eps_th - 1 < this->deg_v && eps_th - 1 >= 0) ? this->nao->at(eps_th - 1).second : -1.0)
    //     << std::endl;
    // std::cerr << "NAO: " << std::endl;
    // for(auto i: *(this->nao)) {
    //     std::cerr << "(" << i.first << "," << i.second << "), ";
    // }
    // std::cerr << std::endl;
    return (eps_th < this->deg_v && this->nao->at(eps_th).second < eps);
}

void NAO::add_update_positive_edge(unsigned long u, double nao) {
    auto it = std::find_if(this->nao->begin(), this->nao->end(), 
        [&u](std::pair<unsigned long, double> a) {
            return a.first == u;
        }
    );
    if (it != this->nao->end()) {
        this->nao->erase(it);
    }
    else {
        this->deg_v++;
        //< this is add a new positive edge operation, so we
        //< need to update the positive degree of the vertex
    }
    auto where = std::lower_bound(this->nao->begin(), this->nao->end(), nao,
        [](std::pair<unsigned long, double> a, double b) {
            return a.second < b;
        }
    );
    //< note that here we compare the second element in the pair
    this->nao->insert(where, std::make_pair(u, nao));
    //< adding the element to its sorted place
}

void NAO::remove_positive_edge(unsigned long u) {
    auto it = std::find_if(this->nao->begin(), this->nao->end(), 
        [&u](std::pair<unsigned long, double> a) {
            return a.first == u;
        }
    );
    if (it != this->nao->end()) {
        this->nao->erase(it);
        this->deg_v--;
    }
    //< otherwise, this edge was not in the positive neighborhood
}

bool sort_order(std::pair<unsigned long, double> a, std::pair<unsigned long, double> b) {
    return a.second < b.second;
}

double NAO::query_na(unsigned long u) {
    auto it = std::find_if(this->nao->begin(), this->nao->end(), 
        [&u](std::pair<unsigned long, double> a) {
            return a.first == u;
        }
    );
    if (it != this->nao->end()) {
        return (*it).second;
    }
    else {
        return INVALID_NON_AGREEMENT;
    }
}