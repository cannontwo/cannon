#include <cannon/ray/mesh.hpp>

#include <cannon/ray/hittable_list.hpp>
#include <cannon/ray/aabb.hpp>
#include <cannon/ray/ray.hpp>

using namespace cannon::ray;

bool Triangle::bounding_box(double /*time_0*/, double /*time_1*/, Aabb& output_box) const {
  Vector3u verts = parent_mesh_->indices_.row(mesh_index_).transpose();

  Vector3d p0 = parent_mesh_->vertices_.row(verts[0]).transpose();
  Vector3d p1 = parent_mesh_->vertices_.row(verts[1]).transpose();
  Vector3d p2 = parent_mesh_->vertices_.row(verts[2]).transpose();

  output_box = surrounding_box(Aabb(p0, p1), p2);
  return true;
}

bool Triangle::object_space_bounding_box(double /*time_0*/, double /*time_1*/, Aabb& output_box) const {
  Vector3u verts = parent_mesh_->indices_.row(mesh_index_).transpose();

  // Transform back to object space
  Vector3d p0 = (*world_to_object_) * parent_mesh_->vertices_.row(verts[0]).transpose();
  Vector3d p1 = (*world_to_object_) * parent_mesh_->vertices_.row(verts[1]).transpose();
  Vector3d p2 = (*world_to_object_) * parent_mesh_->vertices_.row(verts[2]).transpose();

  output_box = surrounding_box(Aabb(p0, p1), p2);
  return true;
}

STAT_COUNTER("Integrator/Triangle hit tests", nTriangleHitTests);
STAT_COUNTER("Integrator/Triangle intersections", nTriangleHits);

bool Triangle::hit(const Ray& r, double /*t_min*/, double t_max, hit_record& rec) const {
  // This implementation from Chapter 3 of PBRT
  
  ++nTriangleHitTests;

  // Transform into ray-local coordinate space.
  // Note this transformation calculation could be cached in Ray, since it's
  // independent of the triangle coords
  Vector3u verts = parent_mesh_->indices_.row(mesh_index_).transpose();
  Vector3d p0 = parent_mesh_->vertices_.row(verts[0]).transpose();
  Vector3d p1 = parent_mesh_->vertices_.row(verts[1]).transpose();
  Vector3d p2 = parent_mesh_->vertices_.row(verts[2]).transpose();

  Vector3d p0t = p0 - r.orig_;
  Vector3d p1t = p1 - r.orig_;
  Vector3d p2t = p2 - r.orig_;

  // Permute coordinates so that the ray's z-axis has the greatest magnitude
  int kz;
  r.dir_.cwiseAbs().maxCoeff(&kz);
  int kx = kz + 1; if (kx == 3) kx = 0; 
  int ky = kx + 1; if (ky == 3) ky = 0;

  Vector3d d = permute_vec(r.dir_, kx, ky, kz);
  p0t = permute_vec(p0t, kx, ky, kz);
  p1t = permute_vec(p1t, kx, ky, kz);
  p2t = permute_vec(p2t, kx, ky, kz);

  // Shear x and y dimensions of vertices to align ray with z axis
  double sx = -d.x() / d.z();
  double sy = -d.y() / d.z();
  double sz = 1.0 / d.z();

  p0t.x() += sx * p0t.z();
  p0t.y() += sy * p0t.z();
  p1t.x() += sx * p1t.z();
  p1t.y() += sy * p1t.z();
  p2t.x() += sx * p2t.z();
  p2t.y() += sy * p2t.z();
  
  // Now intersection testing comes down to checking if (0, 0) lies in the x-y
  // projection of p0t, p1t, p2t
  //
  // For derivation of these coefficients, see pg. 161. 
  double e0 = p1t.x() * p2t.y() - p1t.y() * p2t.x();
  double e1 = p2t.x() * p0t.y() - p2t.y() * p0t.x();
  double e2 = p0t.x() * p1t.y() - p0t.y() * p1t.x();

  if ((e0 < 0 || e1 < 0 || e2 < 0) && (e0 > 0 || e1 > 0 || e2 > 0))
    return false;

  // Ray approaches triangle edge-on, so we don't report intersection
  double det = e0 + e1 + e2;
  if (det == 0)
    return false;

  // Shear z-dimension and check bounds
  p0t.z() *= sz;
  p1t.z() *= sz;
  p2t.z() *= sz;
  double t_scaled = e0 * p0t.z() + e1 * p1t.z() + e2 * p2t.z();
  if (det < 0 && (t_scaled >= 0 || t_scaled < t_max * det))
    return false;
  else if (det > 0 && (t_scaled <= 0 || t_scaled > t_max * det))
    return false;

  // There is an intersection, so we compute barycentric coordinates
  double inv_det = 1.0 / det;
  double b0 = e0 * inv_det;
  double b1 = e1 * inv_det;
  double b2 = e2 * inv_det;
  double t = t_scaled * inv_det;

  // Computing UV coords
  Vector3d dpdu, dpdv;
  Vector2d uv[3];

  uv[0] = parent_mesh_->tex_coords_.row(verts[0]).transpose();
  uv[1] = parent_mesh_->tex_coords_.row(verts[1]).transpose();
  uv[2] = parent_mesh_->tex_coords_.row(verts[2]).transpose();

  Vector2d duv02 = uv[0] - uv[2];
  Vector2d duv12 = uv[1] - uv[2];
  Vector3d dp02 = p0 - p2;
  Vector3d dp12 = p1 - p2;
  
  double determinant = duv02[0] * duv12[1] - duv02[1] * duv12[0];
  if (determinant == 0) {
    // Make an arbitrary coordinate system
    Vector3d v = (p2 - p0).cross(p1 - p0).normalized();
    if (std::abs(v.x()) > std::abs(v.y())) {
      dpdu = Vector3d(-v.z(), 0, v.x()) / std::sqrt(v.x() * v.x() + v.z() * v.z());
    } else {
      dpdu = Vector3d(0, v.z(), -v.y()) / std::sqrt(v.y() * v.y() + v.z() * v.z());
    }
    dpdv = v.cross(dpdu);
  } else {
    double inv_determinant = 1 / determinant;

    dpdu = (duv12[1] * dp02 - duv02[1] * dp12) * inv_determinant;
    dpdv = (-duv12[0] * dp02 + duv02[0] * dp12) * inv_determinant;
  }

  Vector3d pHit = b0 * p0 + b1 * p1 + b2 * p2;
  Vector2d uvHit = b0 * uv[0] + b1 * uv[1] + b2 * uv[2];

  rec.p = pHit;
  rec.u = uvHit[0];
  rec.v = uvHit[1];
  rec.t = t;
  rec.mat_ptr = parent_mesh_->mat_ptr_;

  // Interpolate normals at vertices
  Vector3d ns = b0 * parent_mesh_->normals_.row(verts[0]).transpose() + 
                b1 * parent_mesh_->normals_.row(verts[1]).transpose() + 
                b2 * parent_mesh_->normals_.row(verts[2]).transpose();
  ns.normalize();
  rec.set_face_normal(r, ns);

  nTriangleHits++;
  return true;
}

bool Triangle::object_space_hit(const Ray & /*r*/, double /*t_min*/,
                                double /*t_max*/, hit_record & /*rec*/) const {
  throw std::runtime_error("Triangle object-space intersection should not be called");

  return false;
}

// Public Functions
std::vector<std::shared_ptr<TriangleMesh>>
cannon::ray::load_model(std::shared_ptr<Affine3d> t, std::shared_ptr<Material>
    m, const std::string& path) {
  Assimp::Importer import;
  //const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate |
  //    aiProcess_FlipUVs | aiProcess_GenNormals);
  const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate |
      aiProcess_FlipUVs | aiProcess_GenNormals);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    throw std::runtime_error("Error loading model via Assimp");

  return process_model_node(t, m, scene->mRootNode, scene);
}

std::vector<std::shared_ptr<TriangleMesh>>
cannon::ray::process_model_node(std::shared_ptr<Affine3d> t,
    std::shared_ptr<Material> m, aiNode *node, const aiScene *scene) {
  std::vector<std::shared_ptr<TriangleMesh>> ret_vec;

  // Meshes in this node
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    ret_vec.push_back(process_model_mesh(t, m, mesh, scene));
  }

  // Children of this node
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    auto child_meshes = process_model_node(t, m, node->mChildren[i], scene);
    ret_vec.insert(ret_vec.end(), child_meshes.begin(), child_meshes.end());
  }

  return ret_vec;
}

std::shared_ptr<TriangleMesh>
cannon::ray::process_model_mesh(std::shared_ptr<Affine3d> t, std::shared_ptr<Material> m, aiMesh *mesh,
    const aiScene * /*scene*/) {
  MatrixX3d vertices = MatrixX3d::Zero(mesh->mNumVertices, 3);
  MatrixX3d normals = MatrixX3d::Zero(mesh->mNumVertices, 3);
  MatrixX2d tex_coords = MatrixX2d::Zero(mesh->mNumVertices, 2);
  MatrixX3u indices = MatrixX3u::Zero(mesh->mNumFaces, 3);

  // TODO Load diffuse color or image texture (specular as well?), see graphics/model.cpp
  
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

  return std::make_shared<TriangleMesh>(t, m, vertices, normals, tex_coords, indices);
}

std::shared_ptr<HittableList> cannon::ray::make_mesh_triangle_list(std::shared_ptr<TriangleMesh> mesh) {
  auto triangles = std::make_shared<HittableList>();

  for (int i = 0; i < mesh->indices_.rows(); i++) {
    triangles->add(std::make_shared<Triangle>(mesh->object_to_world_, mesh, i));
  } 

  return triangles;
}

Vector3d cannon::ray::permute_vec(const Vector3d& p, int x, int y, int z) {
  return Vector3d(p[x], p[y], p[z]);
}

