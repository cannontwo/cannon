#ifndef CANNON_GEOM_GRAPH_H
#define CANNON_GEOM_GRAPH_H 

#include <boost/graph/adjacency_list.hpp>

/*!
 * \file cannon/geom/graph.hpp
 * \brief File containing Graph class definition and utility functions.
 */

namespace cannon {
  namespace geom {

    using EdgeProperty = boost::property<boost::edge_weight_t, double>;

    using BGraph =
        boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
                              boost::no_property,
                              EdgeProperty>;

    using Vertex = boost::graph_traits<BGraph>::vertex_descriptor;

    /*!
     * \brief Class representing a directed graph composed of nodes and edges,
     * with edge weights. Primarily a wrapper around Boost graphs.
     */
    class Graph {
    public:
      
      /*!
       * \brief Default constructor.
       */
      Graph() : weights_(get(boost::edge_weight, g_)) {}

      /*!
       * \brief Adds an edge to this graph, implicitly creating the
       * referenced vertices if they don't already exist.
       *
       * \param u "From" vertex
       * \param v "To" vertex
       * \param weight Weight for the edge
       */
      void add_edge(Vertex u, Vertex v, double weight);

      /*!
       * \brief Conduct an A* search on this graph, using the input heuristic.
       *
       * \param start Start vertex for the search.
       * \param goal Goal vertex for the search.
       * \param heur A* heuristic 
       *
       * \returns The shortest path from start to goal, as computed by A*
       */
      std::vector<Vertex> astar(Vertex start, Vertex goal,
                                std::function<double(Vertex)> heur);

      /*!
       * \brief Get the number of vertices in this graph.
       *
       * \returns The number of vertices.
       */
      unsigned int num_vertices() {
        return boost::num_vertices(g_);
      }


    private:
      BGraph g_; //!< Internal graph representation
      boost::property_map<BGraph, boost::edge_weight_t>::type
          weights_; //!< Weight property map
    };

  }
}

#endif /* ifndef CANNON_GEOM_GRAPH_H */
