#ifndef VIENNAGRID_CONFIG_HPP
#define VIENNAGRID_CONFIG_HPP

#include "viennagrid/meta/typemap.hpp"
#include "viennagrid/storage/forwards.hpp"

namespace viennagrid
{
    
    namespace config
    {
        struct topology_config_tag;
        
        struct element_id_tag;
        struct element_container_tag;
        struct element_boundary_storage_layout_tag;
        struct element_appendix_type_tag;
        
        struct id_generator_tag;
        struct continuous_id_generator_tag;
        
        struct coboundary_container_tag;
        struct coboundary_view_container_tag;
    
        
        struct vector_type_tag;
        struct metainfo_typelist_tag;
        struct metainfo_container_config_tag;
        
//         struct segment_id_type_tag;
//         struct segmentation_id_type_tag;
//         struct element_segmentation_tag;
        
        
        
        namespace result_of
        {
            template<typename config_tag>
            struct default_config;
            
            template<>
            struct default_config<id_generator_tag>
            {
                typedef continuous_id_generator_tag type;
            };
            
            template<>
            struct default_config<element_appendix_type_tag>
            {
                typedef viennagrid::meta::null_type type;
            };
            
            template<>
            struct default_config<coboundary_container_tag>
            {
                typedef storage::std_vector_tag type;
            };
            
            template<>
            struct default_config<coboundary_view_container_tag>
            {
                typedef storage::std_vector_tag type;
            };
            
            
            
            
            template<typename config_entry, typename config_tag>
            struct query_config_impl
            {
                typedef typename config_entry::second type;
            };
            
            template<typename config_tag>
            struct query_config_impl< viennagrid::meta::not_found, config_tag >
            {
                typedef typename default_config<config_tag>::type type;
            };

            
            template<typename domain_config, typename config_tag>
            struct query_config
            {
                typedef typename viennagrid::meta::typemap::result_of::find<domain_config, config_tag>::type found;
                typedef typename query_config_impl<found, config_tag>::type type;
            };
        }
    }
    
}


#endif
