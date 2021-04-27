#pragma once
#ifndef CANNON_GRAPHICS_ELEMENT_BUFFER_H
#define CANNON_GRAPHICS_ELEMENT_BUFFER_H 

/*!
 * \file cannon/graphics/element_buffer.hpp
 * File containing ElementBuffer class definition.
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>

#include <cannon/graphics/vertex_array_object.hpp>

using namespace Eigen;

namespace cannon {
  namespace graphics {
    
    using MatrixX3u = Matrix<unsigned int, Dynamic, 3>;
    using Matrix3Xu = Matrix<unsigned int, 3, Dynamic>;

    /*!
     * \brief Class representing an OpenGL element buffer.
     *
     * An element buffer is used to index into other OpenGL vertex buffers, in
     * order to reuse vertex information in drawing geometry.
     *
     * \sa cannon::graphics::VertexBuffer
     */
    class ElementBuffer {
      public:
        ElementBuffer() = delete;

        /*!
         * Constructor taking a vertex array object representing the context in
         * which this element buffer is defined. This element buffer will be
         * active whenever that vertex array object is bound.
         *
         * Does not change OpenGL state.
         *
         * \param vao The vertex array object for this object
         */
        ElementBuffer(std::shared_ptr<VertexArrayObject> vao) : vao_(vao) {
          vao_->bind();
          glGenBuffers(1, &gl_element_buffer_object_);
          vao_->unbind();
        }

        /*!
         * Destructor. If an OpenGL context is active, this method also frees
         * OpenGL resources managed by this object.
         *
         * Sets GL_ELEMENT_ARRAY_BUFFER_BINDING to 0
         */
        ~ElementBuffer() {
          if (glfwGetCurrentContext() != NULL) {
            glDeleteBuffers(1, &gl_element_buffer_object_);
          }
        }

        /*!
         * Method that binds this element buffer to the OpenGL context.
         *
         * Sets GL_VERTEX_ARRAY_BINDING to vao_->gl_vertex_array_object_ and
         * GL_ELEMENT_ARRAY_BUFFER_BINDING to gl_element_buffer_object
         */
        void bind() const;

        /*!
         * Method that unbinds this element buffer from the OpenGL context.
         *
         * Sets GL_VERTEX_ARRAY_BINDING to 0 and
         * GL_ELEMENT_ARRAY_BUFFER_BINDING to 0
         */
        void unbind() const;

        /*!
         * Method that transfers a matrix of indices to the OpenGL buffer
         * encapsulated by this object. Columns of the input matrix represent
         * vertex indices. 
         *
         * \param indices The indices to buffer
         */
        // Does not change OpenGL state
        void buffer(MatrixX3u indices);

      private:
        unsigned int gl_element_buffer_object_; //!< OpenGL ID representing this element buffer
        std::shared_ptr<VertexArrayObject> vao_; //!< The vertex array object for this element buffer
    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_ELEMENT_BUFFER_H */
