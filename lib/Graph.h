/**
 * @file Graph.h
 * @author Ali Shakiba (a.shakiba.iran@gmail.com)
 * @brief Complete signed graph representation by just positive edges
 * @version 0.1
 * @date 2022-12-02
 * @copyright GNU GPLv3
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <unordered_map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cassert>
#include <iostream>
#include <stdexcept>

#define BOOST_LOG_DYN_LINK 1
///< @note Found the following workaround for including system-wide boost library at 
///< https://stackoverflow.com/a/70835728/3006092

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
// #include <boost/log/utility/setup/file.hpp>
// #include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/trivial.hpp>

// #ifndef _INIT_LOGGER
// #define _INIT_LOGGER 1
// void init_logger_graph() {
//     // boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
//     // boost::log::add_file_log(
//     //     boost::log::keywords::file_name = "test-output.log",
//     //     boost::log::keywords::format = "[%TimeStamp%] [%ThreadID%] [%Severity%] %Message%"
//     // );
//     boost::log::core::get()->set_filter(        
//         boost::log::trivial::severity >= boost::log::trivial::error
//     ); 
//     // boost::log::add_common_attributes();
// }
// #endif // _INIT_LOGGER

const double INVALID_NON_AGREEMENT = 5.0;
///< @note When the edge {u,v} is of negative sign, this value would be returned as \textsc{NonAgreement} \in [0,2]

/**
 * @brief This class implements the complete signed graph representation
 * which would be useful for dynamic operations as well as hierarchical 
 * correlation clustering.
 * @details Note that the vertices start as 0 and are of type unsigned long
 * a vertex which is deleted would be marked by making its adjacency list equal 
 * to nullptr.
 * @note Any vertex id is unique and after removing a vertex, its id would not be
 * assigned to any other vertex.
 */
class Graph {
    protected:
        std::unordered_map<unsigned long, std::vector<unsigned long>*> * positive_adjacency;
        ///< @brief Stores the adjacency list of each vertex in a pair of vertex id and
        ///< its adjacent vertices which are guaranteed to be sorted according to their id.

        unsigned long n; 
        ///< @brief The number of vertices

        unsigned long m; 
        ///< @brief The number of edges

        /**
         * @brief A helper method to add a vertex id v into a sorted vector
         * 
         * @param vect a sorted vector we want to add
         * @param v the vertex we want to add to vect
         */
        void add_to_sorted_vector(std::vector<unsigned long> * vect, unsigned long v);
    public:
        /**
         * @brief Construct a new Graph object
         * 
         */
        Graph();
        /**
         * @brief Construct a new Graph object identical to g
         * 
         * @param g 
         */
        Graph(const Graph* g);
        /**
         * @brief Destroy the Graph object
         * 
         */
        ~Graph();
        /**
         * @brief returns the number of adjacent edges with positive sign
         * 
         * @param v the query vertex
         * @return unsigned long deg_{G^+}(v)
         */
        unsigned long deg_positive(unsigned long v);
        /**
         * @brief computes and returns the \textsc{NonAgreement} of the two
         * vertices u and v if they are adjacent by a positive edge. Otherwise,
         * returns INVALID_NON_AGREEMENT
         * 
         * @param u vertex id
         * @param v vertex id
         * @return double either NonAgreement(u,v) or INVALID_NON_AGREEMENT
         */
        double non_agreement(unsigned long u, unsigned long v);
        /**
         * @brief Returns the number of vertices in the graph G
         * 
         * @return unsigned long returns |V|
         */
        unsigned long get_n() { return this->n; };
        /**
         * @brief Returns the number of positive signed edges
         * 
         * @return unsigned long |E^+|
         */
        unsigned long get_positive_m() { return this->m; };
        /**
         * @brief Get the positive neighborhood of a vertex v (read-only)
         * 
         * @param v 
         * @return const std::vector<unsigned long>* |N^+(v)|
         */
        const std::vector<unsigned long> * get_neighborhood(unsigned long v) { return this->positive_adjacency->at(v); };
        /**
         * @brief Adds a positive edge to G
         * 
         * @param u vertex u
         * @param v vertex v
         */
        void add_positive_edge(unsigned long u, unsigned long v);
        /**
         * @brief removes a positive edge {u,v}
         * 
         * @param u vertex u
         * @param v vertex v
         */
        void remove_positive_edge(unsigned long u, unsigned long v);
        /**
         * @brief load the graph from file. 
         * The format of the file is as follows:
         *      - the first line starts with the number of vertices followed by the
         *      number of positive edges, separated by a single space
         *      - all the next lines corresponds to the positive edges like {u,v}
         *      where u and v are the indices of the vertices and start at zero, 
         *      separated by an space
         *      - the edges are undirected, so `1 2` means an edge between vertices
         *      with indices 1 and 2
         * 
         * @see :preprocess.py
         * 
         * @param input exact address to the input file
         */
        void load_from_file(std::string input);
        /**
         * @brief adds a new vertex and returns its id
         * 
         * @return unsigned long the id of the new vertex
         */
        unsigned long add_vertex(); 
        /**
         * @brief removes a vertex v from G
         * 
         * @param v vertex id to be removed
         */
        void remove_vertex(unsigned long v); 
        /**
         * @brief returns the index of todo
         * TODO:
         * @param v 
         * @param query_vertex 
         * @return unsigned long 
         */
        unsigned long where_is_in_neigh_plus(unsigned long v, unsigned long query_vertex); 
        /**
         * @brief true iff query_vertex \in N_+[v]
         * 
         * @param v vertex id
         * @param query_vertex query vertex id
         * @return true iff query_vertex \in N_+[v]
         * @return false otherwise
         */
        bool is_in_neigh_plus(unsigned long v, unsigned long query_vertex);
};

#endif // GRAPH_H_
