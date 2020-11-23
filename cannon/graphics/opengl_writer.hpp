#ifndef CANNON_GRAPHICS_OPENGL_WRITER_H
#define CANNON_GRAPHICS_OPENGL_WRITER_H 

#include <functional>

#include <thirdparty/approval_tests/ApprovalTests.hpp>

#include <cannon/graphics/window.hpp>
#include <cannon/graphics/viewer_3d.hpp>

using namespace ApprovalTests;

namespace cannon {
  namespace graphics {

    class OpenGLWriter : public ApprovalWriter {
      public:

        OpenGLWriter() = delete;

        OpenGLWriter(const Window& w) {
          data_ = w.get_image();
          close_func_ = [&](){w.close();};
        }

        OpenGLWriter(const Viewer3D& v) {
          data_ = v.get_image();
          close_func_ = [&](){v.close();};
        }

        std::string getFileExtensionWithDot() const override;

        void write(std::string path) const override;

        virtual void cleanUpReceived(std::string receivedPath) const override;

      private:
        ImageData data_;
        std::function<const void()> close_func_;


    };

  } // namespace graphics
} // namespace cannon

#endif /* ifndef CANNON_GRAPHICS_OPENGL_WRITER_H */
