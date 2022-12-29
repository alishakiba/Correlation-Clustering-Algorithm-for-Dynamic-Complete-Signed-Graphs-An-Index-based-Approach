#include <gtest/gtest.h>
#include <stdexcept>
#include "../lib/NaiveCorrelationClustering.h"

template<typename T>
void print_vect(const std::vector<T> *vect) {
    for(auto s: *vect)
        std::cerr << s << ", ";
    std::cerr << std::endl;
}

// const std::string input_file = "/home/ali/MyPapers/ReadyPapers/ScalableHierarchicalCorrelationClustering/code/data/simple.txt";
const std::string input_file = "/home/ali/MyPapers/ReadyPapers/ScalableHierarchicalCorrelationClustering/code/data/CA-AstroPh-pre.txt";

class NaiveCCTest : public ::testing::Test {
    protected:
        Graph *g;
        NaiveCorrelationClustering *naive_cc;
        double eps;

        void SetUp() override {
            this->g = new Graph();
            this->g->load_from_file(input_file);
            this->naive_cc = new NaiveCorrelationClustering(this->g);
            eps = 0.8;
        }

        void TearDown() override {
            delete this->g;
            delete this->naive_cc;
        }
};


int main(int argc, char**argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
