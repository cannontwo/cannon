#include <catch2/catch.hpp>

#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>

using namespace cannon::graphics;

TEST_CASE("VertexArrayObject", "[graphics]") {
  Window w;
  OpenGLState s;
  VertexArrayObject vao;
  OpenGLState s1;
  REQUIRE(s == s1);

  vao.bind();
  OpenGLState s2;
  REQUIRE(s2.vertex_array_object != 0);
  s2.vertex_array_object = s1.vertex_array_object;
  REQUIRE(s2 == s1);

  vao.unbind();
  OpenGLState s3;
  REQUIRE(s3 == s1);

  vao.get_next_vertex_attribute_num();
  OpenGLState s4;
  REQUIRE(s4 == s1);
}
