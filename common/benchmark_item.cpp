#include "common/benchmark_item.h"

#include "common/definitions.h"
#include "toml/toml.hpp"

namespace faddeevas
{
    InitArgumentBase::InitArgumentBase(
        std::string const& config_name, toml::node const* ptr_node ) :
        m_config_name( config_name ), m_config_str()
    {
        if( ( ptr_node != nullptr ) && ( ptr_node->as_table() != nullptr ) )
        {
            std::ostringstream a2str;
            a2str << *( ptr_node->as_table() );
            this->m_config_str = a2str.str();
        }
    }

    InitConstArg::InitConstArg( toml::node const* config ):
        InitArgumentBase( "const", config )
    {
        if( ( config != nullptr ) && ( config->as_table() != nullptr ) )
        {
            auto tbl  = *( config->as_table() );
            auto temp = tbl[ "value" ].value< real_type >();

            if( temp.has_value() )
            {
                this->m_value = temp.value();
            }
            else
            {
                this->m_value = real_type{ 0 };
            }

            std::ostringstream a2str;
            a2str << "value = " << this->m_value;
            this->m_config_str = a2str.str();
        }
    }

    InitUniformRandom::InitUniformRandom(
        InitUniformRandom::prng_type& prng,
        toml::node const* config ):
        InitArgumentBase( "uniform_random", config ),
        m_prng( prng )
    {
        if( ( config != nullptr ) && ( config->as_table() != nullptr ) )
        {
            auto tbl = *config->as_table();

            {
                auto temp = tbl[ "min_value" ].value< real_type >();
                this->m_min_value = ( temp.has_value() )
                    ? temp.value() : real_type{ 0 };
            }

            {
                auto temp = tbl[ "max_value" ].value< real_type >();
                this->m_max_value = ( temp.has_value() )
                    ? temp.value() : real_type{ 1 };
            }

            std::ostringstream a2str;
            a2str << "min_value = " << this->m_min_value
                  << "; max_value = " << this->m_max_value;

            this->m_config_str = a2str.str();
        }
    }


    BenchmarkRun::BenchmarkRun( toml::node const& config ) :
        m_prng( BenchmarkRun::seed_type{ 20210618 } )
    {
        if( config.as_table() != nullptr )
        {
            auto tbl = *config.as_table();

            {
                auto int_val = tbl[ "platform_id" ].value< int >();
                this->platform_id = ( int_val.has_value() ) ? int_val.value() : 0;
            }

            {
                auto int_val = tbl[ "device_id" ].value< int >();
                this->device_id = ( int_val.has_value() ) ? int_val.value() : 0;
            }

            {
                auto int_val = tbl[ "default_num_repetitions" ].value< int >();
                this->default_num_repetitions = ( int_val.has_value() )
                    ? int_val.value() : 1;
            }

            {
                auto int_val = tbl[ "default_num_evaluations" ].value< int >();
                this->default_num_evaluations = ( int_val.has_value() )
                    ? int_val.value() : 1;
            }

            {
                auto int_val = tbl[ "default_workgroup_size" ].value< int >();
                this->default_num_evaluations = ( int_val.has_value() )
                    ? int_val.value() : 0;
            }

            {
                auto str_val = tbl[ "path_to_kernel" ].value< std::string >();
                this->path_to_kernel = ( str_val.has_value() )
                    ? str_val.value() : std::string{};
            }

            {
                auto str_val = tbl[ "kernel_file_name" ].value< std::string >();
                this->kernel_file_name = ( str_val.has_value() )
                    ? str_val.value() : std::string{};
            }

            {
                auto str_val = tbl[ "compile_options_pre" ].value< std::string >();
                this->compile_options_pre = ( str_val.has_value() )
                    ? str_val.value() : std::string{};
            }

            {
                auto str_val = tbl[ "compile_options_post" ].value< std::string >();
                this->compile_options_post = ( str_val.has_value() )
                    ? str_val.value() : std::string{};
            }

            {
                auto str_val = tbl[ "path_to_output" ].value< std::string >();
                this->path_to_output = ( str_val.has_value() )
                    ? str_val.value() : std::string{ "./" };
            }

            {
                auto str_val = tbl[ "name" ].value< std::string >();
                this->name = ( str_val.has_value() )
                    ? str_val.value() : std::string{ "benchmark" };
            }

            this->include_dirs.clear();

            if( tbl[ "include_dirs" ].as_array() != nullptr )
            {
                auto include_dirs = *tbl[ "include_dirs" ].as_array();

                for( auto&& idir : include_dirs )
                {
                    if( idir.value< std::string >().has_value() )
                        this->include_dirs.push_back( idir.value<
                            std::string >().value() );
                }
            }
            else
            {
                auto str_val = tbl[ "include_dirs" ].value< std::string >();
                if( str_val.has_value() )
                {
                    this->include_dirs.push_back( str_val.value() );
                }
            }

            if( tbl[ "default_x_arg" ].as_table() != nullptr )
            {
                this->default_init_x_arg = std::move( this->create_init_arg(
                    tbl[ "default_x_arg" ].as_table() ) );
            }

            if( tbl[ "default_y_arg" ].as_table() != nullptr )
            {
                this->default_init_x_arg = std::move( this->create_init_arg(
                    tbl[ "default_y_arg" ].as_table() ) );
            }

            auto neval = tbl[ "num_evaluations" ];

            if( tbl[ "num_evaluations" ].as_array() != nullptr )
            {
                auto neval = *tbl[ "num_evaluations" ].as_array();
                auto nrepetitions = tbl[ "num_repetitions" ].as_array();
                auto wg_sizes = tbl[ "workgroup_size" ].as_array();

                std::size_t const num_entries = neval.size();

                for( std::size_t ii = std::size_t{ 0 } ; ii < num_entries ; ++ii )
                {
                    int const nn = ( neval[ ii ].value< int >().has_value() )
                        ? neval[ ii ].value< int >().value()
                        : this->default_num_evaluations;

                    int const num_rep = ( ( nrepetitions != nullptr ) &&
                        ( nrepetitions->size() >= num_entries ) &&
                        ( (*nrepetitions)[ ii ].value< int >().has_value() ) )
                            ? (*nrepetitions)[ ii ].value< int >().value()
                            : this->default_num_repetitions;

                    int const wg = ( ( wg_sizes != nullptr ) &&
                        ( wg_sizes->size() >= num_entries ) &&
                        ( ( *wg_sizes )[ ii ].value< int >().has_value() ) )
                            ? ( *wg_sizes )[ ii ].value< int >().value()
                            : this->default_workgroup_size;

                    this->items.push_back( BenchmarkItem{} );
                    this->items.back().num_evaluations = nn;
                    this->items.back().num_repetitions = num_rep;
                    this->items.back().workgroup_size  = wg;
                }
            }
            else
            {
                this->items.push_back( BenchmarkItem{} );
                this->items.back().num_evaluations = this->default_num_evaluations;
                this->items.back().num_repetitions = this->default_num_repetitions;
                this->items.back().workgroup_size  = this->default_workgroup_size;
            }

        }
    }

    std::unique_ptr< InitArgumentBase > BenchmarkRun::create_init_arg(
        toml::node const* arg_config_node )
    {
        std::unique_ptr< InitArgumentBase > ptr = std::unique_ptr<
            InitArgumentBase >{ nullptr };

        if( arg_config_node->as_table() != nullptr )
        {
            auto tbl = *arg_config_node->as_table();
            auto tmp = tbl[ "config_name" ].value< std::string >();

            if( tmp.has_value() )
            {
                std::string const config_name = tmp.value();

                if( config_name.compare( "const" ) == 0 )
                {
                    ptr.reset( new InitConstArg( arg_config_node ) );
                }
                else if( config_name.compare( "uniform_random" ) == 0 )
                {
                    ptr.reset( new InitUniformRandom(
                        this->m_prng, arg_config_node ) );
                }
            }
        }

        return ptr;
    }
}
