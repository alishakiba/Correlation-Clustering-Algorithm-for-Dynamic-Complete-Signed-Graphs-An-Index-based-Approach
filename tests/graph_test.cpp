#include <gtest/gtest.h>
#include <stdexcept>
#include"../lib/Graph.h"

const std::string input_file = "/home/ali/MyPapers/ReadyPapers/ScalableHierarchicalCorrelationClustering/code/data/CA-AstroPh-pre.txt";


template<typename T>
void print_vect(const std::vector<T> *vect) {
    for(auto s: *vect)
        std::cerr << s << ", ";
    std::cerr << std::endl;
}


class GraphTest : public ::testing::Test {
    protected:
        Graph *g;

        void SetUp() override {
            this->g = new Graph();
            this->g->load_from_file(input_file);
        }

        void TearDown() override {
            delete this->g;
        }
};

TEST_F(GraphTest, AdjacencyListsAreSortedAndEdgesAreCorrectlyCounted) {
    auto n = g->get_n();
    auto m = g->get_positive_m();
    unsigned long sum = 0;
    for(unsigned long i = 0; i < n; ++i) {
        auto neigh = g->get_neighborhood(i);
        EXPECT_TRUE(std::is_sorted(neigh->begin(), neigh->end()));
        sum += neigh->size();
    }
    ASSERT_EQ(sum, 2 * m);
}

TEST_F(GraphTest, EdgesAreSymmetric) {
    auto n = g->get_n();
    for(unsigned long i = 0u; i < n; ++i) {
        auto neigh = g->get_neighborhood(i);
        for(auto u: *neigh) {
            EXPECT_TRUE(g->is_in_neigh_plus(u, i));
        }
    }
}

TEST_F(GraphTest, AddNewVertex) {
    auto n = g->get_n();
    auto m = g->get_positive_m();
    // adding NEW_VRT new vertices
    unsigned int NEW_VRT = 3u;
    for(unsigned int i = 0; i < NEW_VRT; ++i) {
        auto new_id = g->add_vertex();
        ASSERT_EQ(new_id, n + i); 
    }
    // making sure that number of edges remain the same
    ASSERT_EQ(m, g->get_positive_m());
    // Ensuring their neighborhood is empty
    for(unsigned long i = n; i < n + NEW_VRT; ++i) {
        auto neigh = g->get_neighborhood(i);
        ASSERT_EQ(neigh->size(), 0);
    }
}

TEST_F(GraphTest, AddNewPositiveEdge) {
    auto n = g->get_n();
    // adding new positive edges
    auto edges = {std::make_pair(1u,2u), std::make_pair(5u,7u), std::make_pair(2u,3u)};
    for(auto edge: edges) {
        auto m = g->get_positive_m();
        auto exists = g->is_in_neigh_plus(edge.first, edge.second);
        g->add_positive_edge(edge.first, edge.second);
        auto exists_new = g->is_in_neigh_plus(edge.first, edge.second);
        ASSERT_TRUE(exists_new);
        if (!exists)
            ASSERT_EQ(g->get_positive_m(), m + 1u);
        else
            ASSERT_EQ(g->get_positive_m(), m);
    }
    // no changes should be in the # of vertices
    ASSERT_EQ(n, g->get_n());
}

// I've just added this to make sure the private method works
// Note that I have made this method public for the purpose of
// this test, which then turned back to protected.
// TEST(Graph, AddToSortedVectWorks) {
//     std::vector<unsigned long> vect = {};
//     Graph *g = new Graph();
//     g->add_to_sorted_vector(&vect, 1u);
//     ASSERT_EQ(1, vect.size());
//     g->add_to_sorted_vector(&vect, 2u);
//     ASSERT_EQ(2, vect.size());
//     g->add_to_sorted_vector(&vect, 3u);
//     ASSERT_EQ(3, vect.size());
//     g->add_to_sorted_vector(&vect, 1u);
//     ASSERT_EQ(4, vect.size());
//     ASSERT_TRUE(std::is_sorted(vect.begin(), vect.end()));
// }

TEST_F(GraphTest, RemovingEdges) {
    auto n = g->get_n();
    // removing positive edges
    std::pair<unsigned long, unsigned long> edges[] = {std::make_pair(1u,0u), std::make_pair(3u,0u), std::make_pair(0u,49u)};
    for(auto edge: edges) {
        auto m = g->get_positive_m();
        auto exists = g->is_in_neigh_plus(edge.first, edge.second);
        auto first_deg = g->deg_positive(edge.first);
        auto second_deg = g->deg_positive(edge.second);
        g->remove_positive_edge(edge.first, edge.second);
        auto exists_new = g->is_in_neigh_plus(edge.first, edge.second);
        ASSERT_FALSE(exists_new);
        if (exists) {
            ASSERT_EQ(g->get_positive_m(), m - 1u);
            ASSERT_EQ(g->deg_positive(edge.first), first_deg - 1);
            ASSERT_EQ(g->deg_positive(edge.second), second_deg - 1);
        }
        else {
            ASSERT_EQ(g->get_positive_m(), m);
            ASSERT_EQ(g->deg_positive(edge.first), first_deg);
            ASSERT_EQ(g->deg_positive(edge.second), second_deg);
        }
    }
    // no changes should be in the # of vertices
    ASSERT_EQ(n, g->get_n());
    // what about deleting non-existing edges
    ASSERT_NO_THROW(g->remove_positive_edge(edges[0].first, edges[0].second));
}

TEST_F(GraphTest, RemovingVertices) {
    const unsigned long VERTEX_TO_REMOVE = 10u;
    auto n = g->get_n();
    g->remove_vertex(VERTEX_TO_REMOVE);
    auto n_new = g->get_n();
    ASSERT_EQ(n, n_new + 1);
    n = g->get_n();
    ASSERT_THROW(g->remove_vertex(n+1), std::logic_error);
    auto n_after_except = g->get_n();
    ASSERT_EQ(n, n_after_except);
    ASSERT_EQ(g->get_neighborhood(VERTEX_TO_REMOVE), nullptr);
    ASSERT_THROW(g->deg_positive(VERTEX_TO_REMOVE), std::logic_error);
}

TEST_F(GraphTest, NonAgreement) {
    const unsigned long V = 1lu , U = 0lu;
    auto na = g->non_agreement(U, V);
    ASSERT_NEAR(1.21053, na, 1e-5);
    auto na_inv = g->non_agreement(5lu, 12800lu);
    ASSERT_DOUBLE_EQ(INVALID_NON_AGREEMENT, na_inv);
    // std::cerr << na << std::endl;
    // auto neigh_u = g->get_neighborhood(U);
    // auto neigh_v = g->get_neighborhood(V);
    // auto deg_u = neigh_u->size();
    // auto deg_v = neigh_v->size();
    // print_vect<unsigned long>(neigh_u);
    // print_vect<unsigned long>(neigh_v);
    // std::vector<unsigned long> neigh_intersect((deg_u > deg_v) ? deg_u : deg_v);
    // auto it = std::set_intersection(
    //     neigh_u->begin(), neigh_u->end(), 
    //     neigh_v->begin(), neigh_v->end(), neigh_intersect.begin()
    // );
    // neigh_intersect.resize(it - neigh_intersect.begin());
    // std::cerr << neigh_intersect.size() << std::endl;
    // print_vect<unsigned long>(&neigh_intersect);
}

int main(int argc, char**argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
