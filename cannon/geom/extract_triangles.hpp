#ifndef CANNON_GEOM_EXTRACT_TRIANGLES
#define CANNON_GEOM_EXTRACT_TRIANGLES 

/*!
 * \file cannon/geom/extract_triangles.hpp
 * \brief File containing utilities for extracting triangles from various
 * geometric structures.
 */

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

using K = CGAL::Exact_predicates_exact_constructions_kernel;
using Polygon_2 = CGAL::Polygon_2<K>;

namespace cannon {
  namespace geom {

    /*!
     * \brief Get a vector of triangles triangulating the input polygon,
     * represented as polygons.
     *
     * \param cgal_poly The polygon to triangulate.
     *
     * \returns Vector of triangles triangulating the input polygon.
     */
    std::vector<Polygon_2> extract_triangles(const Polygon_2& cgal_poly);

  }
}

#endif /* ifndef CANNON_GEOM_EXTRACT_TRIANGLES */
