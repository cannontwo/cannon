#ifndef CANNON_RAY_SAMPLER_H
#define CANNON_RAY_SAMPLER_H

/*!
 * \file cannon/ray/sampler.hpp
 * \brief File containing Sampler class definition, subclasses, and helper
 * functions.
 */

#include <Eigen/Dense>

#include <cannon/utils/class_forward.hpp>

using namespace Eigen;

namespace cannon {
  namespace ray {

    CANNON_CLASS_FORWARD(CameraSample);

    /*!
     * \brief Smallest representable number smaller than 1
     */
    static const double ONE_MINUS_EPSILON = 0.99999999999999989;

    /*!
     * \brief Class representing the interface for a sampler used in raytracing.
     * Adapted from PBRT. For each pixel, a set number of random vectors of
     * arbitrary length is generated. 
     */
    class Sampler {
      public:
        /*!
         * \brief Constructor taking number of samples to generate per pixel.
         */
        Sampler(unsigned int samples_per_pixel);

        /*!
         * \brief Method to start sampling for the input pixel.
         *
         * \param p Raster point representing pixel to begin sampling for.
         */
        virtual void start_pixel(const Vector2i& p);

        /*!
         * \brief Method called to start the next sample for the current pixel.
         *
         * \returns True until the number of samples for the current pixel has
         * hit samples_per_pixel_, then false.
         */
        virtual bool start_next_sample();

        /*!
         * \brief Set the sample number for the current pixel.
         *
         * \returns True if sample_num is less than samples_per_pixel_,
         * otherwise false.
         */
        virtual bool set_sample_number(unsigned int sample_num);

        /*!
         * \brief Get the sampled value for the next dimension of the sampled
         * vector.
         *
         * \returns The next sampled value.
         */
        virtual double get_1d() = 0;

        /*!
         * \brief Get the sampled values for the next two dimensions of the
         * sampled vector.
         *
         * \returns The next two sampled values.
         */
        virtual Vector2d get_2d() = 0;

        /*!
         * \brief Use this sampler to generate a camera sample.
         *
         * \param raster_point Pixel raster point to generate sample for.
         * 
         * \returns Camera sample for the input raster pixel.
         */
        CameraSample get_camera_sample(const Vector2i& raster_point);

        /*!
         * \brief Request a one-dimensional array of samples from the sampler.
         * Should be called before sampling begins.
         *
         * \param n Number of samples in the array.
         */
        void request_1d_array(unsigned int n);

        /*!
         * \brief Request a two-dimensional array of samples from the sampler.
         * Should be called before sampling begins.
         *
         * \param n Number of samples in the array.
         */
        void request_2d_array(unsigned int n);

        /*!
         * \brief Get a previously requested array of samples.
         *
         * \param n Expected number of samples to return.
         *
         * \returns Sampled array.
         */
        const double* get_1d_array(unsigned int n);

        /*!
         * \brief Get a previously requested array of sampled vectors.
         *
         * \param n Expected number of samples to return.
         *
         * \returns Sampled array.
         */
        const Vector2d* get_2d_array(unsigned int n);

        /*!
         * \brief Get an adjusted number of samples to generate for sample
         * arrays preferred by this sampler.
         *
         * \param n Requested number of samples
         *
         * \returns Adjusted number of samples to request for array.
         */
        virtual unsigned int round_count(unsigned int n) const;

      protected:
        const unsigned int samples_per_pixel_; //!< Samples to generate per pixel
        Vector2i current_pixel_; //!< Current pixel for sampling
        unsigned int current_pixel_sample_idx_; //!< Current pixel sample number

        std::vector<unsigned int> samples_1d_array_sizes_; //!< Number of samples in 1d arrays
        std::vector<unsigned int> samples_2d_array_sizes_; //!< Number of samples in 2d arrays

        std::vector<std::vector<double>> sample_array_1d_; //!< Allocated samples for 1d sample arrays
        std::vector<std::vector<Vector2d>> sample_array_2d_; //!< Allocated samples for 2d sample arrays

      private:
        size_t array_1d_offset_, array_2d_offset_; //!< Offsets for array access

    };

    /*!
     * \brief Sampler which generates samples for all sample vectors in a pixel
     * at the same time.
     */
    class PixelSampler : public Sampler {
      public:

        /*!
         * \brief Constructor taking the number of samples per pixel and the
         * maximum number of non-array sample dimensions which will be
         * requested.
         */
        PixelSampler(unsigned int samples_per_pixel, unsigned int sampled_dims);

        /*!
         * \brief Inherited from Sampler.
         */
        virtual bool start_next_sample() override;

        /*!
         * \brief Inherited from Sampler.
         */
        virtual bool set_sample_number(unsigned int sample_num) override;

        /*!
         * \brief Inherited from Sampler.
         */
        virtual double get_1d() override;

        /*!
         * \brief Inherited from Sampler.
         */
        virtual Vector2d get_2d() override;

      protected:
        std::vector<std::vector<double>> samples_1d_; //!< Cached 1d samples for the current pixel
        std::vector<std::vector<Vector2d>> samples_2d_; //!< Cached 2d samples for the current pixel
        unsigned int current_1d_dim_ = 0; //!< Current offset into 1d samples
        unsigned int current_2d_dim_ = 0; //!< Current offset into 2d samples

    };

  }
}

#endif /* ifndef CANNON_RAY_SAMPLER_H */
