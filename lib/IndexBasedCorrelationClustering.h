/**
 * @file NaiveCorrelationClustering.h
 * @author Ali Shakiba (a.shakiba.iran@gmail.com)
 * @brief Index-based Correlation Clustering Approximation Algorithm
 * @version 0.1
 * @date 2022-12-15
 * @copyright GNU GPLv3
 */

#ifndef INDEX_BASED_CORRELATION_CLUSTERING_H_
#define INDEX_BASED_CORRELATION_CLUSTERING_H_
#include <stack>
#include <set>
#include <cassert>
#include "NaiveCorrelationClustering.h"
#include "NAO.h"

class IndexBasedCorrelationClustering : protected NaiveCorrelationClustering {
    protected:
        /**
         * @brief storing NAOs for all vertices in the graph 
         * 
         */
        std::unordered_map<unsigned long, NAO*> *naos;
        /**
         * @brief removes all non-agreement edges from g using NAOs
         * 
         * @param eps 
         */
        void remove_non_agree_edges_and_light_with_index(double eps);
    public:
        /**
         * @brief Construct a new Index-based Correlation Clustering object
         * 
         * @param g input graph
         */
        IndexBasedCorrelationClustering(Graph *g);
        /**
         * @brief Destroy the Index-based Correlation Clustering object
         * 
         */
        ~IndexBasedCorrelationClustering();
        /**
         * @brief returns the clustering for 
         * 
         * @param eps 
         * @return std::vector<unsigned long>* a vector of the clustering 
         * function, mapping vertices to clusters
         */
        std::vector<unsigned long>* query(double eps);
        /**
         * @brief recomputes the naos for the g
         * do not forget to call reset_g is required
         * before calling this function
         * 
         */
        void reset_naos();
        /**
         * @brief Get the graph g as a pointer 
         * 
         * @return Graph* 
         */
        Graph *get_g() { return this->g; }; // just for testing
        /**
         * @brief Get the all values of the non-agreements in this graph
         * 
         * @return std::map<double, unsigned long>* 
         */
        std::map<double, unsigned long>* get_all_eps();
};

#endif // INDEX_BASED_CORRELATION_CLUSTERING_H_