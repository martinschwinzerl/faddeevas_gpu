#ifndef FADDEEVAS_OPENCL_COMMON_BENCHMARK_ITEM_H__
#define FADDEEVAS_OPENCL_COMMON_BENCHMARK_ITEM_H__

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <random>

#include "common/definitions.h"
#include "toml/toml.hpp"

namespace faddeevas
{
    struct InitArgumentBase
    {
        typedef std::mt19937_64 prng_type;
        typedef prng_type::result_type  seed_type;
        typedef CERRF_REAL_TYPE real_type;

        explicit InitArgumentBase(
            std::string const& config_name = std::string{},
            toml::node const* ptr_node = nullptr );

        InitArgumentBase( InitArgumentBase const& ) = default;
        InitArgumentBase( InitArgumentBase&& ) = default;

        InitArgumentBase& operator=( InitArgumentBase const& ) = default;
        InitArgumentBase& operator=( InitArgumentBase&& ) = default;

        virtual ~InitArgumentBase() = default;

        virtual void init( real_type* in_begin, real_type* in_end )
        {
            CERRF_ASSERT( in_begin != nullptr );
            CERRF_ASSERT( in_end   != nullptr );

            while( in_begin != in_end ) *in_begin++ = real_type{ 0. };
        }

        std::string m_config_name = std::string{};
        std::string m_config_str  = std::string{};
    };

    struct InitConstArg : public InitArgumentBase
    {
        InitConstArg( toml::node const* config = nullptr );
        InitConstArg( InitConstArg const& ) = default;
        InitConstArg( InitConstArg&& ) = default;

        InitConstArg& operator=( InitConstArg const& ) = default;
        InitConstArg& operator=( InitConstArg&& ) = default;

        virtual ~InitConstArg() = default;

        void init( real_type* in_begin, real_type* in_end ) override
        {
            CERRF_ASSERT( in_begin != nullptr );
            CERRF_ASSERT( in_end   != nullptr );

            while( in_begin != in_end ) *in_begin++ = this->m_value;
        }

        real_type m_value = real_type{ 0 };
    };

    struct InitUniformRandom : public InitArgumentBase
    {
        InitUniformRandom( prng_type& prng, toml::node const* config = nullptr );
        InitUniformRandom( InitUniformRandom const& ) = default;
        InitUniformRandom( InitUniformRandom&& ) = default;

        InitUniformRandom& operator=( InitUniformRandom const& ) = default;
        InitUniformRandom& operator=( InitUniformRandom&& ) = default;

        virtual ~InitUniformRandom() = default;

        void init( real_type* in_begin, real_type* in_end ) override
        {
            CERRF_ASSERT( in_begin != nullptr );
            CERRF_ASSERT( in_end   != nullptr );

            std::uniform_real_distribution< real_type > dist(
                this->m_min_value, this->m_max_value );

            while( in_begin != in_end ) *in_begin++ = dist( this->m_prng );
        }

        prng_type& m_prng;
        real_type  m_min_value = real_type{ 0. };
        real_type  m_max_value = real_type{ 1. };
    };

    /* ********************************************************************* */

    struct BenchmarkItem
    {
        typedef InitArgumentBase::prng_type prng_type;
        typedef InitArgumentBase::seed_type seed_type;

        std::unique_ptr< InitArgumentBase > init_x_arg =
            std::unique_ptr< InitArgumentBase >{};

        std::unique_ptr< InitArgumentBase > init_y_arg =
            std::unique_ptr< InitArgumentBase >{};

        int num_evaluations = 0;
        int num_repetitions = 0;
        int workgroup_size  = 0;
    };

    struct BenchmarkRun
    {
        typedef BenchmarkItem item_type;
        typedef item_type::prng_type prng_type;
        typedef item_type::seed_type seed_type;

        BenchmarkRun( toml::node const& config );

        BenchmarkRun( BenchmarkRun const& ) = default;
        BenchmarkRun( BenchmarkRun&& ) = default;

        BenchmarkRun& operator=( BenchmarkRun const& ) = default;
        BenchmarkRun& operator=( BenchmarkRun&& ) = default;

        virtual ~BenchmarkRun() = default;

        std::unique_ptr< InitArgumentBase > create_init_arg(
            toml::node const* arg_config_node );

        prng_type m_prng;

        std::string path_to_kernel       = std::string{};
        std::string kernel_file_name     = std::string{};
        std::string kernel_fn_name       = std::string{ "eval_wofz" };
        std::string compile_options_pre  = std::string{};
        std::string compile_options_post = std::string{};
        std::string path_to_output       = std::string{};
        std::string name                 = std::string{ "benchmark" };

        std::vector< std::string > include_dirs = std::vector< std::string >{};

        std::vector< BenchmarkItem > items;

        std::unique_ptr< InitArgumentBase > default_init_x_arg =
            std::unique_ptr< InitArgumentBase >{};

        std::unique_ptr< InitArgumentBase > default_init_y_arg =
            std::unique_ptr< InitArgumentBase >{};

        int platform_id = 0;
        int device_id   = 0;

        int default_num_repetitions = 1;
        int default_num_evaluations = 1;
        int default_workgroup_size  = 0;
    };


}

#endif /* FADDEEVAS_OPENCL_COMMON_BENCHMARK_ITEM_H__ */
