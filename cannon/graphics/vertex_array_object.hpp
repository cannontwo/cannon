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
        VertexArrayObject() {
          glGenVertexArrays(1, &gl_vertex_array_object_);
          bind();
        }

        ~VertexArrayObject() {
          unbind();
          glDeleteVertexArrays(1, &gl_vertex_array_object_);
        }

        void bind();
        void unbind();
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
