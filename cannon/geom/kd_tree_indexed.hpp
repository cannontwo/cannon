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

    template <int dim>
    class KDTreeIndexed {
      public:
        using Kernel = CGAL::Epick_d<CGAL::Dimension_tag<dim>>;
        using Point_d = typename Kernel::Point_d;
        using Point_and_int = boost::tuple<Point_d, int>;

        using TraitsBase = CGAL::Search_traits_d<Kernel, CGAL::Dimension_tag<dim>>;
        using Traits = CGAL::Search_traits_adapter<Point_and_int,
              CGAL::Nth_of_tuple_property_map<0, Point_and_int>, TraitsBase>;

        using NeighborSearch = CGAL::Orthogonal_k_neighbor_search<Traits>;
        using Tree = typename NeighborSearch::Tree;

        KDTreeIndexed() {}

        // Each column should be a point to be inserted, matching insertion of
        // a single point being insertion of a column vector
        void insert(const MatrixXd& pts) {
          if (pts.rows() != dim)
            throw std::runtime_error("Matrix of points to be inserted has the wrong number of rows.");

          for (int i = 0; i < pts.cols(); i++) {
            auto col = pts.col(i);
            Point_d tmp(col.data(), col.data() + col.size());
            log_info("Inserting point", tmp);

            Point_and_int tmp_pair = boost::make_tuple(tmp, furthest_idx_ + i);
            tree_.insert(tmp_pair);
          }

          furthest_idx_ += pts.cols();
        }

        std::pair<VectorXd, int> get_nearest_neighbor(const VectorXd& query) {
          if (query.size() != dim)
            throw std::runtime_error("Query vector has the wrong number of rows.");

          Point_d tmp_q(query.data(), query.data() + query.size());
          NeighborSearch search(tree_, tmp_q, 1);

          // Check that we have something to return
          if (search.begin() == search.end())
            throw std::runtime_error("KDTree search didn't return anything.");

          auto ret_pair = search.begin()->first;
          Point_d ret_pt = boost::get<0>(ret_pair);

          VectorXd ret_vec(dim);
          for (int i = 0; i < dim; i++) {
            ret_vec[i] = ret_pt[i];
          }

          return std::make_pair(ret_vec, boost::get<1>(ret_pair));
        }

        int get_nearest_idx(const VectorXd& query) {
          assert(query.size() == dim);
          Point_d tmp_q(query.data(), query.data() + query.size());
          NeighborSearch search(tree_, tmp_q, 1);

          // Check that we have something to return
          if (search.begin() == search.end())
            throw std::runtime_error("KDTree search didn't return anything.");

          auto ret_pair = search.begin()->first;
          return boost::get<1>(ret_pair);
        }

        void clear() {
          tree_.clear();
          furthest_idx_ = 0;
        }

        int get_size() {
          return tree_.size();
        }

      private:
        Tree tree_;
        unsigned int furthest_idx_ = 0;
    };
    
  } // namespace geom
} // namespace cannon

#endif /* ifndef CANNON_GEOM_KD_TREE_INDEXED_H */
