#pragma once
#ifndef CANNON_GRAPHICS_VERTEX_BUFFER_H
#define CANNON_GRAPHICS_VERTEX_BUFFER_H 

/*!
 * \file cannon/graphics/vertex_buffer.hpp
 * \brief Class containing VertexBuffer class definition.
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Eigen/Dense>
#include <iostream>

#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

namespace cannon {
  namespace graphics {

    CANNON_CLASS_FORWARD(VertexArrayObject);

    /*!
     * \brief Class encapsulating an OpenGL vertex buffer object. Handles and
     * represents state of the buffer.
     */
    class VertexBuffer {
      public:

        /*!
         * \brief Default constructor.
         */
        VertexBuffer() : vao_(nullptr) {}

        /*!
         * \brief Constructor taking a VertexArrayObject that this VertexBuffer
         * will be associated with. This VertexBuffer will have a vertex
         * attribute number allocated by the input VAO.
         */
        VertexBuffer(VertexArrayObjectPtr vao);

        VertexBuffer(VertexBuffer& buf) = delete;

        /*!
         * \brief Move constructor. 
         */
        VertexBuffer(VertexBuffer&& buf) :
          gl_vertex_buffer_object_(buf.gl_vertex_buffer_object_),
          gl_vertex_attribute_num_(buf.gl_vertex_attribute_num_),
          vao_(buf.vao_) {
            buf.gl_vertex_buffer_object_ = -1;
          }

        /*!
         * \brief Destructor. Cleans up OpenGL vertex buffer object if possible.
         */
        ~VertexBuffer() {
          if (glfwGetCurrentContext() != NULL) {
            if (vao_ != nullptr)
              bind();

            glDeleteBuffers(1, &gl_vertex_buffer_object_);

            unbind();
          }
        }

        /*!
         * \brief Method to initialize this vertex buffer with a VAO.
         *
         * Does not affect OpenGL State
         *
         * \param vao The VertexArrayObject to associate this vertex buffer with.
         */
        void init(VertexArrayObjectPtr vao);

        /*!
         * \brief Method to bind this verex buffer for drawing. 
         *
         * Sets GL_VERTEX_ARRAY_BINDING to vao_->gl_vertex_array_object_ and
         * GL_ARRAY_BUFFER_BINDING to gl_vertex_buffer_object
         */
        void bind() const;

        /*!
         * \brief Method to unbind this vertex buffer.
         *
         * Sets GL_VERTEX_ARRAY_BINDING to 0 and GL_ARRAY_BUFFER_BINDING to 0
         */
        void unbind() const;

        /*!
         * \brief Method to add data to this vertex buffer. 
         *
         * Does not affect OpenGL state
         */
        void buffer(MatrixX2f vertices);

        /*!
         * \brief Method to add data to this vertex buffer. 
         *
         * Does not affect OpenGL state
         */
        void buffer(MatrixX3f vertices);

        /*!
         * \brief Method to add data to this vertex buffer. 
         *
         * Does not affect OpenGL state
         */
        void buffer(MatrixX4f vertices);

        /*!
         * \brief Method to replace the data in this vertex buffer. Can be more
         * efficient than buffer() if the new data has the same size as the old
         * data.
         *
         * Does not affect OpenGL state
         */
        void replace(MatrixX2f vertices);

        /*!
         * \brief Method to replace the data in this vertex buffer. Can be more
         * efficient than buffer() if the new data has the same size as the old
         * data.
         *
         * Does not affect OpenGL state
         */
        void replace(MatrixX3f vertices);

        /*!
         * \brief Method to replace the data in this vertex buffer. Can be more
         * efficient than buffer() if the new data has the same size as the old
         * data.
         *
         * Does not affect OpenGL state
         */
        void replace(MatrixX4f vertices);

        friend std::ostream& operator<<(std::ostream&, const VertexBuffer&);

      private:

        /*!
         * \brief Method to set the vertex attribute number for this vertex buffer.
         *
         * \param dim Dimension of vertices in this vertex buffer.
         */
        void set_vertex_attribute_pointer(int dim); 

        unsigned int gl_vertex_buffer_object_; //!< OpenGL object managed by this class
        int gl_vertex_attribute_num_; //!< Vertex attribute number for this vertex buffer
        VertexArrayObjectPtr vao_; //!< VAO associated with this buffer
    };

    std::ostream& operator<<(std::ostream&, const VertexBuffer&);

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_VERTEX_BUFFER_H */
