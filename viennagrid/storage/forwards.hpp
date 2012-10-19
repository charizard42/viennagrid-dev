#ifndef VIENNAGRID_STORAGE_FORWARDS_HPP
#define VIENNAGRID_STORAGE_FORWARDS_HPP


namespace viennagrid
{
    
    namespace storage
    {
        
        // reference
//         struct pointer_reference_tag {};
//         
//         template<typename container_type>
//         struct iterator_reference_tag {};
//         
//         template<typename container_type>
//         struct id_reference_tag {};


        // hooks
        struct no_hook_tag {};
        struct iterator_hook_tag {};
        struct pointer_hook_tag {};
        
        template<typename id_type>
        struct id_hook_tag {};

        
        
        
        
        // container
        struct default_tag {};
        
        struct std_vector_tag {};
        struct std_deque_tag {};
        struct std_list_tag {};
        struct std_set_tag {};
        
        
        
        template<typename container_tag__, typename hook_tag__>
        struct hooked_container_tag
        {
            typedef container_tag__ container_tag;
            typedef hook_tag__ hook_tag;
        };

        
    }
    
}

#endif