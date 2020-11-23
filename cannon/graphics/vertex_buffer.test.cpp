#include <catch2/catch.hpp>

#include <cannon/graphics/vertex_buffer.hpp>
#include <cannon/graphics/opengl_state.hpp>
#include <cannon/graphics/window.hpp>

using namespace cannon::graphics;

TEST_CASE("VertexBuffer", "[graphics]") {
  Window w;
  auto vao = std::make_shared<VertexArrayObject>();
  OpenGLState s;

  VertexBuffer buf;
  OpenGLState s1;
  REQUIRE(s == s1);

  buf.init(vao);
  OpenGLState s2;
  REQUIRE(s == s2);

  buf.bind();
  OpenGLState s3;
  REQUIRE(s3.vertex_array_object != 0);
  REQUIRE(s3.array_buffer != 0);
  s3.vertex_array_object = s.vertex_array_object;
  s3.array_buffer = s.array_buffer;
  REQUIRE(s3 == s);
  
  buf.unbind();
  OpenGLState s4;
  REQUIRE(s4 == s);

  MatrixX2f m = MatrixX2f::Zero(1, 2);
  buf.buffer(m);
  OpenGLState s5;
  REQUIRE(s5 == s);

  buf.replace(m);
  OpenGLState s6;
  REQUIRE(s6 == s);

  MatrixX3f m1 = MatrixX3f::Zero(1, 3);
  buf.buffer(m1);
  OpenGLState s7;
  REQUIRE(s7 == s);

  buf.replace(m1);
  OpenGLState s8;
  REQUIRE(s8 == s);

  MatrixX4f m2 = MatrixX4f::Zero(1, 4);
  buf.buffer(m2);
  OpenGLState s9;
  REQUIRE(s9 == s);

  buf.replace(m2);
  OpenGLState s10;
  REQUIRE(s10 == s);
}
