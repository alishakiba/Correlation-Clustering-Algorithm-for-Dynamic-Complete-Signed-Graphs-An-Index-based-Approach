#include <gtest/gtest.h>
#include <stdexcept>
#include"../lib/NAO.h"

template<typename T>
void print_vect(const std::vector<T> *vect) {
    for(auto s: *vect)
        std::cerr << s << ", ";
    std::cerr << std::endl;
}

// const std::string input_file = "/home/ali/MyPapers/ReadyPapers/ScalableHierarchicalCorrelationClustering/code/data/simple.txt";
const std::string input_file = "/home/ali/MyPapers/ReadyPapers/ScalableHierarchicalCorrelationClustering/code/data/CA-AstroPh-pre.txt";

class SimpleGraphTest : public ::testing::Test {
    protected:
        Graph *g;
        std::unordered_map<unsigned long, NAO*> *naos;
        double eps;

        void SetUp() override {
            this->g = new Graph();
            this->g->load_from_file(input_file);
            auto n = this->g->get_n();
            this->naos = new std::unordered_map<unsigned long, NAO*>();
            for(unsigned long i = 0ul; i < n; ++i) {
                // std::cerr << "HERE!!!!!" << std::endl;
                this->naos->insert(std::make_pair(i, new NAO(i, this->g)));
            }
            this->eps = 0.8;
        }

        void TearDown() override {
            delete this->g;
            for(auto e: *(this->naos))
                delete e.second;
            delete this->naos;
        }
};

TEST_F(SimpleGraphTest, NAOs_AreSorted) {
    for(auto e: *(this->naos)) {
        auto nao = e.second->get_nao();
        auto is_sorted = std::is_sorted(nao->begin(), nao->end(), [](std::pair<unsigned long, double> a, std::pair<unsigned long, double> b) {
            return sort_order(a, b);
        });
        EXPECT_TRUE(is_sorted);
    }
}


TEST_F(SimpleGraphTest, SingleNaoTest) {
    const unsigned long VERTEX = 0ul;
    auto n = this->g->get_n();
    auto nao_obj = this->naos->at(VERTEX);
    auto nao = nao_obj->get_nao();
    for(auto e: *nao) {
        ASSERT_DOUBLE_EQ(e.second, nao_obj->query_na(e.first));
    }
    for(unsigned long i = 0ul; i < n; ++i) {
        auto val = nao_obj->query_na(i);
        auto it = std::find_if(nao->begin(), nao->end(), [&i](std::pair<unsigned long, double> a) {
                return a.first == i;
        });
        if (val == INVALID_NON_AGREEMENT) {
            ASSERT_TRUE(it == nao->end());
        }
        else {
            ASSERT_DOUBLE_EQ(val, (*it).second);
            ASSERT_TRUE(it != nao->end());
        }
    }
}


TEST_F(SimpleGraphTest, NAOsEqualNonAgreement) {
    auto n = this->g->get_n();
    for(unsigned long i = 0; i < n; ++i) {
        for(auto j: *(this->g->get_neighborhood(i))) {
            ASSERT_DOUBLE_EQ(
                this->g->non_agreement(i, j), this->naos->at(i)->query_na(j)
            );
        }
    }
}


TEST_F(SimpleGraphTest, NAOsEqualLightness) {
    auto n = this->g->get_n();

    auto eps_agree_cnt = new std::vector<unsigned long>(this->g->get_n(), 0);
    auto is_light = new std::vector<bool>(this->g->get_n(), false);
    auto eps_agree_neigh = new std::vector<std::vector<unsigned long>*>(this->g->get_n(), nullptr);
    for(unsigned long i = 0ul; i < this->g->get_n(); ++i) {
        (*eps_agree_neigh)[i] = new std::vector<unsigned long>();
    }
    // counting the # of e-agreement positive edges
    for(unsigned long i = 0ul; i < this->g->get_n(); ++i) {
        for(auto j: *(this->g->get_neighborhood(i))) {
            if (this->g->non_agreement(i, j) < this->eps) {
                // (i, j) are in eps-agreement
                (*eps_agree_cnt)[i]++;
                (*eps_agree_cnt)[j]++;
                (*eps_agree_neigh)[i]->push_back(j);
                (*eps_agree_neigh)[j]->push_back(i);
            }
        }
    }
    // as the edges are undirected, you need to consider one side
    for(unsigned long i = 0ul; i < this->g->get_n(); ++i) {
        ASSERT_TRUE((*eps_agree_cnt)[i] % 2 == 0);
        (*eps_agree_cnt)[i] /= 2;
    }
    // identifying whether vertices are e-light or not
    for(unsigned long i = 0ul; i < this->g->get_n(); ++i) {
        if (
            this->g->deg_positive(i) == 0
            ||
            (*eps_agree_cnt)[i] < this->eps * this->g->deg_positive(i)
        ) {
            (*is_light)[i] = true;
        }
    }
    // is this equal for NAOs?
    for(unsigned long i = 0; i < n; ++i) {
        // std::cerr << "Node " << i << ": " 
        //     << this->eps * this->g->deg_positive(i) << ", " 
        //     << ((*eps_agree_cnt)[i] < this->eps * this->g->deg_positive(i)) << ", " 
        //     << ((this->g->deg_positive(i) > 0) ? (*eps_agree_cnt)[i] / static_cast<double>(this->g->deg_positive(i)) : -1)
        //     << std::endl;
        // std::cerr << "Plain: " << std::endl;
        // for(auto j: *(*eps_agree_neigh)[i]) {
        //     std::cerr << "(" << j << "," << this->g->non_agreement(i,j) << "), ";
        // }
        // std::cerr << std::endl;
        ASSERT_EQ(
            is_light->at(i), ! this->naos->at(i)->is_heavy(this->eps)
        );
        // std::cerr << "************************" << std::endl;
    }
}


int main(int argc, char**argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
