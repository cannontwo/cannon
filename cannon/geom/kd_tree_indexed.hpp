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

namespace cannon {
  namespace geom {

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

        KDTreeIndexed(int dim) : dim_(dim) {}

        // Each column should be a point to be inserted, matching insertion of
        // a single point being insertion of a column vector
        void insert(const MatrixXd& pts);
        void clear();

        std::pair<VectorXd, int> get_nearest_neighbor(const VectorXd& query);
        int get_nearest_idx(const VectorXd& query);
        int get_size();

      private:
        int dim_;
        Tree tree_;
        unsigned int furthest_idx_ = 0;
    };
    
  } // namespace geom
} // namespace cannon

#endif /* ifndef CANNON_GEOM_KD_TREE_INDEXED_H */
