#ifndef CANNON_GRAPHICS_VERTEX_ARRAY_OBJECT_H
#define CANNON_GRAPHICS_VERTEX_ARRAY_OBJECT_H 

#include <glad/glad.h>
#include <iostream>

#include <cannon/log/registry.hpp>

using namespace cannon::log;

namespace cannon {
  namespace graphics {

    class VertexArrayObject {
      public:
        // Does not change OpenGL state
        VertexArrayObject() {
          glGenVertexArrays(1, &gl_vertex_array_object_);
        }

        VertexArrayObject(VertexArrayObject& vao) = delete;

        VertexArrayObject(VertexArrayObject&& vao) :
          gl_vertex_array_object_(vao.gl_vertex_array_object_),
          declared_vertex_attribs_(vao.declared_vertex_attribs_) {
            vao.gl_vertex_array_object_ = -1;
        } 

        ~VertexArrayObject() {
          try {
            unbind();
            glDeleteVertexArrays(1, &gl_vertex_array_object_);
          } catch (...) {
            // This may fail if the OpenGL context is destroyed, which is fine 
          }
        }

        // Sets GL_VERTEX_ARRAY_BINDING to gl_vertex_array_object_
        void bind();

        // Sets GL_VERTEX_ARRAY_BINDING to 0
        void unbind();

        // Does not affect OpenGL state
        int get_next_vertex_attribute_num();

        friend std::ostream& operator<<(std::ostream&, const VertexArrayObject&);

      private:
        unsigned int gl_vertex_array_object_;
        int declared_vertex_attribs_ = 0;
    };

    std::ostream& operator<<(std::ostream&, const VertexArrayObject&);

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_VERTEX_ARRAY_OBJECT_H */
