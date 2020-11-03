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

    template <int dim>
    class KDTree {
      public:
        using Kernel = CGAL::Epick_d<CGAL::Dimension_tag<dim>>;
        using Point_d = typename Kernel::Point_d;
        using Traits = CGAL::Search_traits_d<Kernel, CGAL::Dimension_tag<dim>>;
        using NeighborSearch = CGAL::Orthogonal_k_neighbor_search<Traits>;
        using Tree = typename NeighborSearch::Tree;

        KDTree() {}

        // Each column should be a point to be inserted, matching insertion of
        // a single point being insertion of a column vector
        void insert(const MatrixXd& pts) {
          if (pts.rows() != dim)
            throw std::runtime_error("Matrix of points to be inserted has the wrong number of rows.");

          for (int i = 0; i < pts.cols(); i++) {
            auto col = pts.col(i);
            Point_d tmp(col.data(), col.data() + col.size());
            log_info("Inserting point", tmp);
            tree_.insert(tmp);
          }
        }

        VectorXd get_nearest_neighbor(const VectorXd& query) {
          if (query.size() != dim)
            throw std::runtime_error("Query vector has the wrong number of rows.");
          Point_d tmp_q(query.data(), query.data() + query.size());
          NeighborSearch search(tree_, tmp_q, 1);

          // Check that we have something to return
          if (search.begin() == search.end())
            throw std::runtime_error("KDTree search didn't return anything.");

          Point_d ret_pt = search.begin()->first;
          VectorXd ret_vec(dim);
          for (int i = 0; i < dim; i++) {
            ret_vec[i] = ret_pt[i];
          }

          return ret_vec;
        }

        void clear() {
          tree_.clear();
        }

        int get_size() {
          return tree_.size();
        }

      private:
        Tree tree_;
    };
    
  } // namespace geom
} // namespace cannon

#endif /* ifndef CANNON_GEOM_KD_TREE_H */
