#include <cannon/graphics/geometry/model.hpp>

using namespace cannon::graphics;
using namespace cannon::graphics::geometry;

void Model::draw(const Matrix4f& view, const Matrix4f& perspective) const {
  for (auto& mesh : meshes_) {
    mesh->set_parent_model_mat(get_model_mat());
    mesh->draw(view, perspective);
  }
}

void Model::draw(std::shared_ptr<ShaderProgram> p, const Matrix4f& view, const
    Matrix4f& perspective) const {
  for (auto& mesh : meshes_) {
    mesh->set_parent_model_mat(get_model_mat());
    mesh->draw(p, view, perspective);
  }
}

void Model::load_model_(const std::string& path) {
  Assimp::Importer import;
  const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    throw std::runtime_error("Error loading model via Assimp");

  directory_ = path.substr(0, path.find_last_of('/'));

  log_info("Model directory is", directory_);

  process_node_(scene->mRootNode, scene);
}

void Model::process_node_(aiNode *node, const aiScene *scene) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes_.push_back(process_mesh_(mesh, scene));
  }

  // TODO Also handle defining Mesh parent/child relationships (position/rotation)
  for (unsigned int i = 0; i < node->mNumChildren; i++)
    process_node_(node->mChildren[i], scene);
}

std::shared_ptr<Mesh> Model::process_mesh_(aiMesh *mesh, const aiScene *scene) {
  MatrixX3f vertices(mesh->mNumVertices, 3);
  MatrixX3f normals(mesh->mNumVertices, 3);
  MatrixX2f tex_coords(mesh->mNumVertices, 2);
  MatrixX3u indices(mesh->mNumFaces, 3);

  std::vector<std::shared_ptr<Texture>> diffuse;
  std::vector<std::shared_ptr<Texture>> specular;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    vertices.row(i)[0] = mesh->mVertices[i].x;
    vertices.row(i)[1] = mesh->mVertices[i].y;
    vertices.row(i)[2] = mesh->mVertices[i].z;

    normals.row(i)[0] = mesh->mNormals[i].x;
    normals.row(i)[1] = mesh->mNormals[i].y;
    normals.row(i)[2] = mesh->mNormals[i].z;

    if (mesh->mTextureCoords[0]) {
      tex_coords.row(i)[0] = mesh->mTextureCoords[0][i].x;
      tex_coords.row(i)[1] = mesh->mTextureCoords[0][i].y;
    } else {
      tex_coords.row(i)[0] = 0.0;
      tex_coords.row(i)[1] = 0.0;
    }

  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    indices.row(i)[0] = mesh->mFaces[i].mIndices[0];
    indices.row(i)[1] = mesh->mFaces[i].mIndices[1];
    indices.row(i)[2] = mesh->mFaces[i].mIndices[2];
  }

  if (mesh->mMaterialIndex >= 0) {
    log_info("Loading mesh material");
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    diffuse = load_material_textures_(material, aiTextureType_DIFFUSE);
    specular = load_material_textures_(material, aiTextureType_SPECULAR);
  }
  
  return std::make_shared<Mesh>(program, vertices, normals, tex_coords,
      indices, diffuse, specular);
}

std::vector<std::shared_ptr<Texture>> Model::load_material_textures_(aiMaterial *mat, 
    aiTextureType type) {
  std::vector<std::shared_ptr<Texture>> textures;

  log_info("Material has", mat->GetTextureCount(type), "textures of type", type);
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);
    std::string path_str(str.C_Str());
    path_str = directory_ + "/" + path_str;
    
    textures.push_back(Texture::load_texture(path_str));
  }

  return textures;
}
