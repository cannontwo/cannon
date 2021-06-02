#pragma once
#ifndef CANNON_MATH_LATTICE_POINTS_H
#define CANNON_MATH_LATTICE_POINTS_H 

/*!
 * \file cannon/math/lattice_points.hpp
 * \brief File containing utilities for working with points making up a lattice
 * in arbitrary real vector spaces.
 */

#include <queue>

#include <Eigen/Dense>

using namespace Eigen;

namespace cannon {
  namespace math {

      using VectorXu = Matrix<unsigned int, Dynamic, 1>;

      /*!
       * \brief Generate all lattice points on an integer-valued grid.
       *
       * \param dim Which dimension of the lattice to generate.
       * \param sizes Number of elements in each dimension of the grid.
       *
       * \return Lattice points for dimensions 0 through dim-1
       */
      std::vector<VectorXu> make_lattice_points(unsigned int dim, const VectorXu& sizes) {
        std::vector<VectorXu> ret_vec;

        if (dim == 1) {
          for (unsigned int i = 0; i < sizes[dim-1]; i++) {
            VectorXu tmp = VectorXu::Zero(dim);
            tmp[0] = i;
            ret_vec.push_back(tmp);
          }
        } else {
          for (unsigned int i = 0; i < sizes[dim-1]; i++) {
            auto smaller_vec = make_lattice_points(dim-1, sizes);
            for (const auto& coord : smaller_vec) {
              VectorXu tmp = VectorXu::Zero(dim);
              tmp.head(dim-1) = coord;
              tmp[dim-1] = i;

              ret_vec.push_back(tmp);
            }
          }
        }

        return ret_vec;
      }

      /*!
       * \brief Generate all lattice points on an integer-valued grid.
       *
       * \param sizes Number of elements in each dimension of the grid.
       *
       * \return Lattice points for the grid.
       */
      std::vector<VectorXu> make_lattice_points(const VectorXu& sizes) {
        return make_lattice_points(sizes.size(), sizes);
      }

  } // namespace math
} // namespace cannon


#endif /* ifndef CANNON_MATH_LATTICE_POINTS_H */
