#include <gtest/gtest.h>
#include <stdexcept>
#include <algorithm>

#include "../lib/NaiveCorrelationClustering.h"
#include "../lib/IndexBasedCorrelationClustering.h"

template<typename T>
void print_vect(const std::vector<T> *vect) {
    for(auto s: *vect)
        std::cerr << s << ", ";
    std::cerr << std::endl;
}

// const std::string input_file = "/home/ali/MyPapers/ReadyPapers/ScalableHierarchicalCorrelationClustering/code/data/simple.txt";
const std::string input_file = "/home/ali/MyPapers/ReadyPapers/ScalableHierarchicalCorrelationClustering/code/data/CA-AstroPh-pre.txt";

class IndexCCTest : public ::testing::Test {
    protected:
        Graph *g;
        NaiveCorrelationClustering *naive_cc;
        IndexBasedCorrelationClustering *index_cc;
        double eps;

        void SetUp() override {
            this->g = new Graph();
            this->g->load_from_file(input_file);
            this->naive_cc = new NaiveCorrelationClustering(this->g);
            this->index_cc = new IndexBasedCorrelationClustering(this->g);
            eps = 0.8;
        }

        void TearDown() override {
            delete this->g;
            delete this->naive_cc;
            delete this->index_cc;
        }
};

TEST_F(IndexCCTest, Same_Clustering) {
    auto naive_output = this->naive_cc->query(eps);
    auto index_output = this->index_cc->query(eps);
    for(unsigned long i = 0ul; i < this->g->get_n(); ++i) {
        ASSERT_EQ(naive_output->at(i), index_output->at(i));
    }
}

TEST_F(IndexCCTest, Same_Deleted_Edges) {
    this->naive_cc->reset_g();
    this->index_cc->reset_naos();
    // making sure that the underlying graphs are the same
    // initially
    auto naive_g =  this->naive_cc->get_g();
    auto index_g = this->index_cc->get_g();
    ASSERT_EQ(naive_g->get_n(), index_g->get_n());
    ASSERT_EQ(naive_g->get_positive_m(), index_g->get_positive_m());
    std::cerr << "Before Clustering:" << std::endl;
    for(unsigned long i = 0; i < naive_g->get_n(); ++i) {
        auto neigh_naive = *(naive_g->get_neighborhood(i));
        auto neigh_index = *(index_g->get_neighborhood(i));
        ASSERT_EQ(neigh_naive.size(), neigh_index.size());
        for(unsigned long j = 0; j < neigh_naive.size(); ++j) {
            ASSERT_EQ(neigh_naive[j], neigh_index[j]);
        }
    }
    // removing edges 
    auto naive_output = this->naive_cc->query(eps);
    auto index_output = this->index_cc->query(eps);
    // verifying if the graphs are the same, again
    std::cerr << "After Clustering:" << std::endl;
    naive_g =  this->naive_cc->get_g();
    index_g = this->index_cc->get_g();
    ASSERT_EQ(naive_g->get_n(), index_g->get_n());
    auto m_naive = 0ul, m_index = 0ul;
    for(unsigned long i = 0; i < naive_g->get_n(); ++i) {
        auto neigh_naive = *(naive_g->get_neighborhood(i));
        m_naive += neigh_naive.size();
        auto neigh_index = *(index_g->get_neighborhood(i));
        m_index += neigh_index.size();
        ASSERT_EQ(neigh_naive.size(), neigh_index.size());
        for(unsigned long j = 0; j < neigh_naive.size(); ++j) {
            ASSERT_EQ(neigh_naive[j], neigh_index[j]);
        }
    }
    ASSERT_EQ(m_index, m_naive);
    ASSERT_EQ(m_naive / 2, naive_g->get_positive_m());
    ASSERT_EQ(m_index / 2, index_g->get_positive_m());
    ASSERT_EQ(naive_g->get_positive_m(), index_g->get_positive_m());
}


int main(int argc, char**argv) {
    ::testing::InitGoogleTest(&argc, argv);
    // init_logger_graph();
    return RUN_ALL_TESTS();
}
