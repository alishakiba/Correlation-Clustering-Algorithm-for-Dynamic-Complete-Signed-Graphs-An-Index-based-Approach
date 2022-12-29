/**
 * @file NAO.h
 * @author Ali Shakiba (a.shakiba.iran@gmail.com)
 * @brief NonAgreement Ordering Index Structure
 * @version 0.1
 * @date 2022-12-02
 * @copyright GNU GPLv3
 */

#ifndef NAO_H_
#define NAO_H_

#include<vector>
#include<cmath>
#include "Graph.h"

class NAO {
    protected:
        std::vector<std::pair<unsigned long, double>> *nao; 
        ///< this stores the adjacent positive neighbors in increasing
        ///< order of their non-agreement with vertex v
        unsigned long v; 
        ///< this is NAO(v)
        unsigned long deg_v; 
        ///< deg_v in G^+
    public:
        /**
         * @brief Construct a new NAO for vertex v using the graph g
         * 
         * @param v vertex index in graph G
         * @param g the graph G
         */
        NAO(unsigned long v, Graph *g);
        /**
         * @brief Destroy the NAO object
         * 
         */
        ~NAO();
        /**
         * @brief queries the index and returns all vertices whose 
         * non-agreement is less than eps
         * 
         * @param eps parameter eps
         * @return std::vector<std::pair<unsigned long, double>> the set of vertices whose non-agreement
         * is less than eps 
         */
        std::vector<std::pair<unsigned long, double>> query(double eps);
        /**
         * @brief gets the non-agreement of a vertex u with current vertex v
         * 
         * @param u target vertex
         * @return double the non-agreement, if not connected it would be INVALID_NON_AGREEMENT
         */
        double query_na(unsigned long u);
        /**
         * @brief returns true iff vertex v is eps-heavy
         * 
         * @param eps parameter eps
         * @return true if this vertex is eps-heavy
         * @return false if this vertex is eps-light
         */
        bool is_heavy(double eps);
        /**
         * @brief a new positive edge is added {v,u} or its non-agreement value is updated
         * 
         * @param u the other endpoint of the new edge
         * @param noa the value of u and v non-agreement
         */
        void add_update_positive_edge(unsigned long u, double noa);
        /**
         * @brief removes an existing positive edge {v,u}
         * 
         * @param u the other endpoint of the new edge
         */
        void remove_positive_edge(unsigned long u);
        /**
         * @brief Get the nao object
         * 
         * @return const std::vector<std::pair<unsigned long, double>>* 
         */
        const std::vector<std::pair<unsigned long, double>> * get_nao() {
            return this->nao;
        }
};

/**
 * @brief internal function used for sorting the elements
 * 
 * @param a first element
 * @param b second element
 * @return true if a <= b
 * @return false otherwise
*/
bool sort_order(std::pair<unsigned long, double> a, std::pair<unsigned long, double> b);

#endif // NAO_H_