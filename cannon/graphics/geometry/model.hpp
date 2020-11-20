#ifndef CANNON_GRAPHICS_GEOMETRY_MODEL_H
#define CANNON_GRAPHICS_GEOMETRY_MODEL_H 

#include <vector>
#include <string>
#include <memory>
#include <stdexcept>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cannon/graphics/geometry/drawable_geom.hpp>
#include <cannon/graphics/geometry/mesh.hpp>
#include <cannon/graphics/texture.hpp>
#include <cannon/graphics/shader_program.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::log;

namespace cannon {
  namespace graphics {
    namespace geometry {
      
      class Model : public DrawableGeom {
        public:
          Model() = delete;

          Model(std::shared_ptr<ShaderProgram> p, const std::string& path) {
            program = p;
            load_model_(path);

            name_ = std::string("Model");
          }

          virtual void draw(const Matrix4f& view, const Matrix4f& perspective) const override;
          virtual void draw(std::shared_ptr<ShaderProgram> p, const Matrix4f&
              view, const Matrix4f& perspective) const override;

          virtual void write_imgui(int idx) override;

        private:
          void load_model_(const std::string& path);
          void process_node_(aiNode *node, const aiScene *scene);
          std::shared_ptr<Mesh> process_mesh_(aiMesh *mesh, const aiScene *scene);
          std::vector<std::shared_ptr<Texture>> load_material_textures_(aiMaterial *mat, 
              aiTextureType type);

          std::vector<std::shared_ptr<Mesh>> meshes_;
          std::string directory_;

      };

    } // namespace geometry
  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_GEOMETRY_MODEL_H */
