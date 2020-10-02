#ifndef CANNON_GRAPHICS_VERTEX_ARRAY_OBJECT_H
#define CANNON_GRAPHICS_VERTEX_ARRAY_OBJECT_H 

#include <glad/glad.h>

namespace cannon {
  namespace graphics {

    class VertexArrayObject {
      public:
        VertexArrayObject() {
          glGenVertexArrays(1, &gl_vertex_array_object_);
        }

        ~VertexArrayObject() {
          glDeleteVertexArrays(1, &gl_vertex_array_object_);
        }

        void bind();
        void unbind();

      private:
        unsigned int gl_vertex_array_object_;
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_VERTEX_ARRAY_OBJECT_H */
