#ifndef CANNON_GEOM_KD_TREE_INDEXED_H
#define CANNON_GEOM_KD_TREE_INDEXED_H 

#include <utility>
#include <stdexcept>

#include <Eigen/Core>
#include <CGAL/Epick_d.h>
#include <CGAL/Search_traits_d.h>
#include <CGAL/Orthogonal_k_neighbor_search.h>
#include <CGAL/Search_traits_adapter.h>
#include <CGAL/property_map.h>
#include <boost/tuple/tuple.hpp>

#include <cannon/log/registry.hpp>

using namespace Eigen;
using namespace cannon::log;

/*!
 * \file cannon/geom/kd_tree_indexed.hpp
 * File containing KDTreeIndexed class definition.
 *
 */

namespace cannon {
  namespace geom {

    /*!
     * \brief Class representing a kd-tree which keeps track of the index of inserted points.
     * 
     * This class is an extension on top of a straightforward kd-tree that
     * associates an index with each inserted point. This is useful if the
     * inserted points are also kept in some structure outside of the kd-tree,
     * in which case storing the index for each point obviates the need to
     * search for retrieved nearest neighbor points.
     *
     * \sa cannon::geom::KDTree
     */
    class KDTreeIndexed {
      public:
        using Kernel = CGAL::Epick_d<CGAL::Dynamic_dimension_tag>;
        using Point_d = typename Kernel::Point_d;
        using Point_and_int = boost::tuple<Point_d, int>;

        using TraitsBase = CGAL::Search_traits_d<Kernel, CGAL::Dynamic_dimension_tag>;
        using Traits = CGAL::Search_traits_adapter<Point_and_int,
              CGAL::Nth_of_tuple_property_map<0, Point_and_int>, TraitsBase>;

        using NeighborSearch = CGAL::Orthogonal_k_neighbor_search<Traits>;
        using Tree = typename NeighborSearch::Tree;

        KDTreeIndexed() = delete;

        /*!
         * Constructor taking the dimensionality of points that this kd-tree will hold.
         *
         * \param dim The dimension of points to be held.
         */
        KDTreeIndexed(int dim) : dim_(dim) {}

        /*!
         * Method to insert points into the kd-tree. 
         *
         * \param pts Matrix of points to be added to the kd-tree. Each column
         * should be a point to be inserted, matching insertion of a single
         * point being insertion of a column vector
         */
        void insert(const MatrixXd& pts);

        /*! 
         * Method to clear this kd-tree, removing all held points.
         */
        void clear();

        /*!
         * Method to query the nearest neighbor of the input point.
         *
         * \param query Point to find nearest neighbor of.
         *
         * \returns A pair consisting of the closest previously inserted point
         * to the query point and its index.
         */
        std::pair<VectorXd, int> get_nearest_neighbor(const VectorXd& query);

        /*!
         * Method to get the index of the nearest neighbor of the input point.
         *
         * \param query Point to find nearest neighbor of.
         *
         * \returns The index of the nearest neighbor.
         */
        int get_nearest_idx(const VectorXd& query);

        /*!
         * Method to get the number of points in this kd-tree.
         *
         * \returns Number of points in this tree.
         */
        int get_size();

      private:
        int dim_; //!< Dimension of points in this kd-tree
        Tree tree_; //!< Internal CGAL kd-tree object
        unsigned int furthest_idx_ = 0; //!< Greatest index assigned to a point in this kd-tree so far
    };
    
  } // namespace geom
} // namespace cannon

#endif /* ifndef CANNON_GEOM_KD_TREE_INDEXED_H */
