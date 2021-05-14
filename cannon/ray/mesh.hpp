#pragma once
#ifndef CANNON_RAY_MESH_H
#define CANNON_RAY_MESH_H 

/*!
 * \file cannon/ray/mesh.hpp
 * \brief File containing TriangleMesh and Triangle class definitions.
 */

#include <vector>
#include <string>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Eigen/Dense>

#include <cannon/ray/hittable.hpp>
#include <cannon/ray/hittable_list.hpp>
#include <cannon/log/registry.hpp>
#include <cannon/utils/statistics.hpp>

using namespace Eigen;

using namespace cannon::log;
using namespace cannon::utils;

namespace cannon {
  namespace ray {

    using MatrixX3u = Matrix<unsigned int, Dynamic, 3>;
    using Vector3u = Matrix<unsigned int, 3, 1>;

    /*!
     * \brief Class representing a mesh composed of a collection of triangles.
     */
    class TriangleMesh {
      public:

        TriangleMesh() = delete;
        
        /*!  
         * Constructor taking object to world transform, vertices, normals,
         * texture coords, and face indices defining this mesh.
         */
        TriangleMesh(std::shared_ptr<Affine3d> object_to_world,
            std::shared_ptr<Material> mat, const MatrixX3d& vertices, const
            MatrixX3d& normals, const MatrixX2d& tex_coords, const MatrixX3u&
            indices) : object_to_world_(object_to_world), mat_ptr_(mat),
        vertices_(vertices), normals_(normals), tex_coords_(tex_coords),
        indices_(indices) {

            // Transform all vertices and normals to world space to save on ray testing computation
            assert(vertices.rows() == normals.rows());

            for (unsigned int i = 0; i < vertices.rows(); i++) {
              vertices_.row(i) = (*object_to_world) * vertices.row(i).transpose();
              normals_.row(i) = object_to_world->linear() * normals.row(i).transpose();
            }
          }


      public:
        std::shared_ptr<Affine3d> object_to_world_; //!< Object to world transform for this mesh
        std::shared_ptr<Material> mat_ptr_; //!< Material for this mesh

        MatrixX3d vertices_; //!< Vertices
        MatrixX3d normals_; //!< Normals
        MatrixX2d tex_coords_; //!< Texture coordinates (UV)
        MatrixX3u indices_; //!< Indices into vertices/normals for each face

    };

    /*!
     * \brief Class representing a single triangle hittable.
     */
    class Triangle : public Hittable {
      public:

        Triangle() = delete;

        /*!
         * Constructor taking object_to_world transform, parent mesh, and face
         * index within mesh.
         */
        Triangle(std::shared_ptr<Affine3d> object_to_world,
            std::shared_ptr<TriangleMesh> mesh, int mesh_index) :
          Hittable(object_to_world), parent_mesh_(mesh),
          mesh_index_(mesh_index) {}

        /*!
         * Destructor.
         */
        virtual ~Triangle() {}
        
        /*!
         * Inherited from Hittable.
         */
        virtual bool bounding_box(double time_0, double time_1, Aabb& output_box) const override;

        /*!
         * Inherited from Hittable.
         */
        virtual bool object_space_bounding_box(double time_0, double time_1, Aabb& output_box) const override;

        /*!
         * Inherited from Hittable.
         */
        virtual bool hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

        /*!
         * Inherited from Hittable.
         */
        virtual bool object_space_hit(const Ray& r, double t_min, double t_max, hit_record& rec) const override;

      public:
        std::shared_ptr<TriangleMesh> parent_mesh_; //!< Mesh that this triangle is a part of
        int mesh_index_; //!< Index of this triangle in the parent mesh

    };

    // Public Functions
    
    /*!
     * Function to load a collection of meshes from an object file.
     *
     * \param t Object-to-world transform for the loaded model.
     * \param m Material for loaded model (not loading textures from file for now).
     * \param path The file to load.
     *
     * \returns A vector of meshes loaded from the file.
     */
    std::vector<std::shared_ptr<TriangleMesh>>
      load_model(std::shared_ptr<Affine3d> t, std::shared_ptr<Material> m, const std::string& path);

    /*!
     * Function to process an Assimp model node into a list of meshes.
     *
     * \param t Object-to-world transform for the loaded model.
     * \param m Material for loaded model.
     * \param node The node to process
     * \param scene The overall Assimp scene
     * 
     * \returns A vector of meshes loaded from the node.
     */
    std::vector<std::shared_ptr<TriangleMesh>>
      process_model_node(std::shared_ptr<Affine3d> t, std::shared_ptr<Material> m, aiNode *node, const
          aiScene *scene);

    /*!
     * Function to process a single Assimp mesh
     *
     * \param t Object-to-world transform for the loaded model.
     * \param m Material for loaded mesh
     * \param mesh The mesh to process
     * \param scene The overall Assimp scene
     *
     * \returns The processed TriangleMesh
     */
    std::shared_ptr<TriangleMesh> process_model_mesh(std::shared_ptr<Affine3d>
        t, std::shared_ptr<Material> m, aiMesh *mesh, const aiScene *scene);

    /*!
     * Create list of hittable triangles for the input mesh.
     *
     * \param mesh The triangle mesh to process into triangles
     *
     * \returns The list of hittables.
     */
    std::shared_ptr<HittableList> make_mesh_triangle_list(std::shared_ptr<TriangleMesh> mesh);

    /*!
     * Permute the coordinates of an input vector.
     *
     * \param p The vector to permute.
     * \param x Index of new x coordinate in orginal vector.
     * \param y Index of new y coordinate in orginal vector.
     * \param z Index of new z coordinate in orginal vector.
     *
     * \return The permuted vector.
     */
    Vector3d permute_vec(const Vector3d& p, int x, int y, int z);
    

  } // namespace ray
} // namespace cannon

#endif /* ifndef CANNON_RAY_MESH_H */
