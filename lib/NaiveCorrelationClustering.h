/**
 * @file NaiveCorrelationClustering.h
 * @author Ali Shakiba (a.shakiba.iran@gmail.com)
 * @brief Naive Correlation Clustering Approximation Algorithm
 * @version 0.1
 * @date 2022-12-11
 * @copyright GNU GPLv3
 */

#ifndef NAIVE_CORRELATION_CLUSTERING_H_
#define NAIVE_CORRELATION_CLUSTERING_H_
#include <stack>
#include <cassert>
#include "Graph.h"

class NaiveCorrelationClustering {
    protected:
        Graph *g;
        Graph *g_temp;
        const Graph *original_g;
        /**
         * @brief removes all non-agreement edges from g
         * 
         * @param eps 
         */
        void remove_non_agree_edges_and_light(double eps);
        /**
         * @brief compuetes the connected components of the graph
         * and return it, the cluster numebrs start at 1
         * 
         * @return std::vector<unsigned long>* cluster assignment function starting at 1
         */
        std::vector<unsigned long>* connected_components();
    public:
        /**
         * @brief Construct a new Naive Correlation Clustering object
         * 
         * @param g input graph
         */
        NaiveCorrelationClustering(Graph *g);
        /**
         * @brief Destroy the Naive Correlation Clustering object
         * 
         */
        ~NaiveCorrelationClustering();
        /**
         * @brief returns the clustering for 
         * 
         * @param eps 
         * @return std::vector<unsigned long>* a vector of the clustering 
         * function, mapping vertices to clusters
         */
        std::vector<unsigned long>* query(double eps);
        /**
         * @brief copies the graph original_g to g
         * 
         */
        void reset_g();
        Graph *get_g() { return this->g; }; // just for testing
};

#endif // NAIVE_CORRELATION_CLUSTERING_H_