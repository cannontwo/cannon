#ifndef CANNON_RAY_SAMPLER_H
#define CANNON_RAY_SAMPLER_H

/*!
 * \file cannon/ray/sampler.hpp
 * \brief File containing Sampler class definition, subclasses, and helper
 * functions.
 */

#include <Eigen/Dense>

#include <cannon/utils/class_forward.hpp>
#include <cannon/math/random_double.hpp>

using namespace Eigen;

using namespace cannon::math;

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
         * \brief Destructor.
         */
        virtual ~Sampler();

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
     * at the same time, on a per-pixel basis.
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

    /*!
     * \brief Sampler interface which is not pixel-based, but which rather
     * generates samples across the entire image. 
     */
    class GlobalSampler : public Sampler {
      public:

        /*!
         * \brief Constructor taking number of samples per pixel.
         */
        GlobalSampler(unsigned int samples_per_pixel);

        /*!
         * \brief Compute the inverse mapping between a sample number in the
         * current pixel and a global sample index.
         *
         * \param sample_num Sample number in the current pixel.
         *
         * \returns Global sample index.
         */
        virtual unsigned int get_index_for_sample(unsigned int sample_num) const = 0;

        /*!
         * \brief Get the input dimension of the input sample index.
         *
         * \param index Global sample index.
         * \param dimension Dimension of sample to return.
         *
         * \returns Sampled value.
         */
        virtual double sample_dimension_(unsigned int index, unsigned int dimension) const = 0;

        /*!
         * \brief Inherited from Sampler.
         */
        void start_pixel(const Vector2i& p) override;

        /*!
         * \brief Inherited from Sampler.
         */
        bool start_next_sample() override;

        /*!
         * \brief Inherited from Sampler.
         */
        bool set_sample_number(unsigned int sample_num) override;

        /*!
         * \brief Inherited from Sampler.
         */
        double get_1d() override;

        /*!
         * \brief Inherited from Sampler.
         */
        Vector2d get_2d() override;

      private:
        unsigned int dimension_; //!< Next dimension that sampler will be asked to generate
        unsigned int interval_sample_index_; //!< Global index of the current sample in the current pixel

        static const unsigned int array_start_dim_ = 5; //!< Start dimension for array samples
        unsigned int array_end_dim_; //!< End dimension for array samples

    };

    class StratifiedSampler : public PixelSampler {
      public:
        /*!
         * \brief Constructor taking number of x and y pixel samples, whether
         * to jitter samples, and the number of sampled dimensions to be
         * generated.
         */
        StratifiedSampler(unsigned int x_pixel_samples,
                          unsigned int y_pixel_samples, bool jitter_samples,
                          int n_sampled_dimensions);

        /*!
         * \brief Inherited from Sampler.
         */
        void start_pixel(const Vector2i& p) override;

      private:

        /*!
         * \brief Compute 1D stratified samples and place them in the input array.
         *
         * \param sample_vec Vector in which to place samples
         * \param n_samples Number of samples to generate
         * \param jitter Whether to jitter samples
         */
        void stratified_sample_1d_(double *sample_vec, unsigned int n_samples, bool jitter);

        /*!
         * \brief Compute 2D stratified samples and place them in the input array.
         *
         * \param sample_vec Vector in which to place samples
         * \param jitter Whether to jitter samples
         */
        void stratified_sample_2d_(std::vector<Vector2d> &sample_vec, bool jitter);

        /*!
         * \brief Randomly shuffle entries of the input vector, which contains the input
         * count number of elements with the input number of dimensions.
         *
         * \param sample_vec The vector to shuffle
         * \param count The number of elements to shuffle
         * \param n_dims The number of dimensions in each shuffled element.
         */
        template <typename T>
        void shuffle_(T *sample_vec, unsigned int count, unsigned int n_dims) {
          for (size_t i = 0; i < count; ++i) {
            unsigned int other = i + std::floor(random_double(0.0, count - i));
            for (unsigned int j = 0; j < n_dims; ++j) {
              std::swap(sample_vec[n_dims * i + j],
                        sample_vec[n_dims * other + j]);
            }
          }
        }

        /*!
         * \brief Generate Latin Hypercube samples for stratified array sampling.
         *
         * \param sample_vec Vector in which to place samples
         * \param n_samples Number of samples
         * \param n_dim Number of dimensions per sample
         */
        void latin_hypercube_(std::vector<double> &sample_vec,
                              unsigned int n_samples, unsigned int n_dim);

        const unsigned int x_pixel_samples_; //!< Number of X dimension pixel samples
        const unsigned int y_pixel_samples_; //!< Number of Y dimension pixel samples
        const bool jitter_samples_; //!< Whether stratified samples should be jittered

    };

  }
}

#endif /* ifndef CANNON_RAY_SAMPLER_H */
