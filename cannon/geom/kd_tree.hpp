#ifndef CANNON_GEOM_KD_TREE_H
#define CANNON_GEOM_KD_TREE_H 

#include <stdexcept>

#include <Eigen/Core>
#include <CGAL/Epick_d.h>
#include <CGAL/Search_traits_d.h>
#include <CGAL/Orthogonal_k_neighbor_search.h>

#include <cannon/log/registry.hpp>

using namespace Eigen;
using namespace cannon::log;

namespace cannon {
  namespace geom {

    class KDTree {
      public:
        using Kernel = CGAL::Epick_d<CGAL::Dynamic_dimension_tag>;
        using Point_d = typename Kernel::Point_d;
        using Traits = CGAL::Search_traits_d<Kernel, CGAL::Dynamic_dimension_tag>;
        using NeighborSearch = CGAL::Orthogonal_k_neighbor_search<Traits>;
        using Tree = typename NeighborSearch::Tree;

        KDTree() = delete;

        KDTree(int dim) : dim_(dim) {}

        // Each column should be a point to be inserted, matching insertion of
        // a single point being insertion of a column vector
        void insert(const MatrixXd& pts);
        void clear();

        VectorXd get_nearest_neighbor(const VectorXd& query);
        int get_size();

      private:
        int dim_;
        Tree tree_;
    };
    
  } // namespace geom
} // namespace cannon

#endif /* ifndef CANNON_GEOM_KD_TREE_H */
