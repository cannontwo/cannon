#pragma once
#ifndef CANNON_GRAPHICS_OPENGL_WRITER_H
#define CANNON_GRAPHICS_OPENGL_WRITER_H 

/*!
 * \file cannon/graphics/opengl_writer.hpp
 * File containing OpenGLWriter class definition.
 */

#include <functional>

#include <thirdparty/approval_tests/ApprovalTests.hpp>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/viewer_3d.hpp>
#include <cannon/plot/plotter.hpp>

using namespace ApprovalTests;
using namespace cannon::plot;

namespace cannon {
  namespace graphics {

    /*!
     * \brief Class enabling the use of ApprovalTests with OpenGL rendering.
     *
     * This class is a utility to enable more comprehensive testing of the
     * cannon::graphics package. It is responsible for writing an OpenGL state
     * to a file, which is then compared using ApprovalTests.
     */
    class OpenGLWriter : public ApprovalWriter {
      public:

        OpenGLWriter() = delete;

        /*!
         * Constructor taking a raw window.
         */
        OpenGLWriter(const Window& w) {
          data_ = w.get_image();
          close_func_ = [&](){w.close();};
        }

        /*!
         * Constructor taking a Viewer3D, which is essentially a decorated window.
         */
        OpenGLWriter(const Viewer3D& v) {
          data_ = v.get_image();
          close_func_ = [&](){v.close();};
        }

        /*!
         * Constructor taking a Plotter, which is essentially a decorated window.
         */
        OpenGLWriter(const Plotter& p) {
          data_ = p.get_image();
          close_func_ = [&](){p.close();};
        }

        /*!
         * Method required by ApprovalTests. Returns the file extension that
         * the OpenGL screenshot should be saved with. In this case, we are
         * using PNG.
         */
        std::string getFileExtensionWithDot() const override;

        /*!
         * Method required by ApprovalTests. Does the actual writing of image
         * data to the input file path.
         */
        void write(std::string path) const override;

        /*!
         * Method required by ApprovalTests. Closes the OpenGL window used to
         * construct this writer.
         */
        virtual void cleanUpReceived(std::string receivedPath) const override;

      private:
        ImageData data_; //!< Actual image data to be written to a file

        std::function<const void()> close_func_; //!< Function to be called once writing is complete

    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_OPENGL_WRITER_H */
