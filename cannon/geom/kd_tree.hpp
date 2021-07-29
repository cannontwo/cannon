#ifndef CANNON_GEOM_KD_TREE_H
#define CANNON_GEOM_KD_TREE_H 

/*!
 * \file cannon/geom/kd_tree.hpp
 * \brief File containing KDTree class definition.
 */

/*!
 * \namespace cannon::geom
 * \brief Namespace containing computational geometry classes and functions,
 * largely wrapping CGAL.
 */

#include <Eigen/Core>
#include <CGAL/Epick_d.h>
#include <CGAL/Search_traits_d.h>
#include <CGAL/Orthogonal_k_neighbor_search.h>

using namespace Eigen;

namespace cannon {
  namespace geom {

    /*!
     * \brief Class representing a kd-tree.
     * 
     * A kd-tree can be used for nearest neighbor searches. This class
     * primarily wraps the same functionality in CGAL. 
     * 
     * \sa cannon::geom::KDTreeIndexed
     */
    class KDTree {
      public:
        using Kernel = CGAL::Epick_d<CGAL::Dynamic_dimension_tag>;
        using Point_d = typename Kernel::Point_d;
        using Traits = CGAL::Search_traits_d<Kernel, CGAL::Dynamic_dimension_tag>;
        using NeighborSearch = CGAL::Orthogonal_k_neighbor_search<Traits>;
        using Tree = typename NeighborSearch::Tree;

        KDTree() = delete;

        /*!
         * Constructor taking the dimensionality of points that this kd-tree will hold.
         *
         * \param dim The dimension of points to be held.
         */
        KDTree(int dim) : dim_(dim) {}

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
         * \returns The closest previously inserted point to the query point.
         */
        VectorXd get_nearest_neighbor(const VectorXd& query);

        /*!
         * Method to get the number of points in this kd-tree.
         *
         * \returns Number of points in this tree.
         */
        int get_size();

      private:
        int dim_; //!< Dimension of points in this kd-tree
        Tree tree_; //!< Internal CGAL kd-tree object
    };
    
  } // namespace geom
} // namespace cannon

#endif /* ifndef CANNON_GEOM_KD_TREE_H */
