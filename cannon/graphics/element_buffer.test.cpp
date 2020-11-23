#include <catch2/catch.hpp>

#include <cannon/graphics/element_buffer.hpp>
#include <cannon/graphics/window.hpp>
#include <cannon/graphics/opengl_state.hpp>
#include <cannon/graphics/vertex_array_object.hpp>
#include <cannon/log/registry.hpp>

using namespace cannon::graphics;
using namespace cannon::log;

TEST_CASE("ElementBuffer", "[graphics]") {
  Window w;
  OpenGLState s;

  auto vao = std::make_shared<VertexArrayObject>();
  ElementBuffer e(vao);
  OpenGLState s1;
  REQUIRE(s == s1);

  e.bind();
  OpenGLState s2;
  REQUIRE(s2.element_buffer != 0);
  REQUIRE(s2.vertex_array_object != 0);

  // Check that element_buffer and vao are the only things that changed.
  s2.vertex_array_object = s.vertex_array_object;
  s2.element_buffer = s.element_buffer;
  REQUIRE(s2 == s);

  e.unbind();
  OpenGLState s3;
  REQUIRE(s == s3);

  MatrixX3u m(1, 3);
  m << 0.0, 0.0, 0.0;
  e.buffer(m);
  OpenGLState s4;
  REQUIRE(s == s4);
}
