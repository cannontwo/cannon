#ifndef CANNON_GRAPHICS_GEOMETRY_MODEL_H
#define CANNON_GRAPHICS_GEOMETRY_MODEL_H 

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <cannon/graphics/geometry/drawable_geom.hpp>
#include <cannon/utils/class_forward.hpp>

namespace cannon {
  namespace graphics {

    CANNON_CLASS_FORWARD(ShaderProgram);
    CANNON_CLASS_FORWARD(Texture);

    namespace geometry {

      CANNON_CLASS_FORWARD(Mesh);
      
      class Model : public DrawableGeom {
        public:
          Model() = delete;

          Model(ShaderProgramPtr p, const std::string& path) {
            program = p;
            load_model_(path);

            name_ = std::string("Model");
          }

          // Does not affect OpenGL state
          virtual void draw(const Matrix4f& view, const Matrix4f& perspective) const override;

          // Does not affect OpenGL state
          virtual void draw(ShaderProgramPtr p, const Matrix4f& view, const
              Matrix4f& perspective) const override;

          // Does not affect OpenGL state
          virtual void write_imgui(int idx) override;

        private:
          void load_model_(const std::string& path);
          void process_node_(aiNode *node, const aiScene *scene);
          MeshPtr process_mesh_(aiMesh *mesh, const aiScene *scene);
          std::vector<TexturePtr> load_material_textures_(aiMaterial *mat, 
              aiTextureType type);

          std::vector<MeshPtr> meshes_;
          std::string directory_;

      };

    } // namespace geometry
  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_GEOMETRY_MODEL_H */
