#pragma once
#ifndef CANNON_GRAPHICS_VERTEX_ARRAY_OBJECT_H
#define CANNON_GRAPHICS_VERTEX_ARRAY_OBJECT_H 

/*!
 * \file cannon/graphics/vertex_array_object.hpp
 * \brief File containing VertexArrayObject class definition.
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace cannon {
  namespace graphics {

    /*!
     * \brief Class encapsulating an OpenGL Vertex Array Object. Responsible
     * for binding and unbinding the encapsulated OpenGL object. Note that this
     * class cannot be copied because there would be harmful interactions
     * between multiple VertexArrayObjects with the same associated OpenGL
     * object.
     */
    class VertexArrayObject {
      public:

        /*!
         * Default Constructor.
         *
         * Does not change OpenGL state.
         */
        VertexArrayObject() {
          glGenVertexArrays(1, &gl_vertex_array_object_);
        }

        VertexArrayObject(VertexArrayObject& vao) = delete;

        /*!
         * Move constructor.
         */
        VertexArrayObject(VertexArrayObject&& vao) :
          gl_vertex_array_object_(vao.gl_vertex_array_object_),
          declared_vertex_attribs_(vao.declared_vertex_attribs_) {
            vao.gl_vertex_array_object_ = -1;
        } 

        /*!
         * Destructor, which frees OpenGL resources if possible.
         */
        ~VertexArrayObject() {
          if (glfwGetCurrentContext() != NULL) {
            unbind();
            glDeleteVertexArrays(1, &gl_vertex_array_object_);
          }
        }

        /*!
         * Method to bind the associated OpenGL Vertex Array Object.
         *
         * Sets GL_VERTEX_ARRAY_BINDING to gl_vertex_array_object_.
         */
        void bind();

        /*!
         * Method to unbind the associated OpenGL Vertex Array Object.
         *
         * Sets GL_VERTEX_ARRAY_BINDING to 0.
         */
        void unbind();

        /*!
         * Method to get next vertex attribute number for use with vertex buffers.
         *
         * Does not affect OpenGL state.
         *
         * \returns Next unallocated vertex attribute number.
         * 
         * \sa cannon::graphics::VertexBuffer
         */
        int get_next_vertex_attribute_num();

        friend std::ostream& operator<<(std::ostream&, const VertexArrayObject&);

      private:
        unsigned int gl_vertex_array_object_; //!< OpenGL vertex array object
        int declared_vertex_attribs_ = 0; //!< Number of already allocated vertex attributes for this VAO.
    };

    /*!
     * Operator implementation for debug printing.
     */
    std::ostream& operator<<(std::ostream&, const VertexArrayObject&);

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_VERTEX_ARRAY_OBJECT_H */
