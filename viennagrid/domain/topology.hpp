#ifndef VIENNAGRID_TOPOLOGIC_DOMAIN_HPP
#define VIENNAGRID_TOPOLOGIC_DOMAIN_HPP

/* =======================================================================
   Copyright (c) 2011-2012, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                     ViennaGrid - The Vienna Grid Library
                            -----------------

   Authors:      Karl Rupp                           rupp@iue.tuwien.ac.at
                 Josef Weinbub                    weinbub@iue.tuwien.ac.at
               
   (A list of additional contributors can be found in the PDF manual)

   License:      MIT (X11), see file LICENSE in the base directory
======================================================================= */

#include "viennagrid/forwards.hpp"

#include "viennagrid/meta/algorithm.hpp"

#include "viennagrid/storage/id_generator.hpp"
#include "viennagrid/storage/inserter.hpp"
#include "viennagrid/storage/algorithm.hpp"

#include "viennagrid/config/element_config.hpp"


namespace viennagrid
{

    template<typename container_collection_type_, typename inserter_type_>
    class topology_t
    {
    public:
        typedef container_collection_type_ container_collection_type;
        typedef inserter_type_ inserter_type;
        
        topology_t() : container_collection(), inserter() {}
        topology_t(const topology_t & other) : container_collection(other.container_collection), inserter(other.inserter)
        {
            inserter.set_container_collection( container_collection );
        }
        
        ~topology_t()
        {
            viennadata::erase<viennadata::all, viennadata::all>()(*this);
        }
        
        topology_t & operator=(const topology_t & other)
        {
            container_collection = other.container_collection;
            inserter = other.inserter;
            inserter.set_container_collection( container_collection );
            return *this;
        }
        
        container_collection_type & get_container_collection() { return container_collection; }
        container_collection_type const & get_container_collection() const { return container_collection; }
        
        inserter_type & get_inserter() { return inserter; }
        inserter_type const & get_inserter() const { return inserter; }
        
    protected:
        container_collection_type container_collection;
        inserter_type inserter;
    };
}


namespace viennagrid
{
    
    namespace result_of
    {
        template<typename something>
        struct container_collection_typemap
        {
            typedef typename container_collection_typemap< typename container_collection<something>::type >::type type;
        };
        
        template<typename typemap>
        struct container_collection_typemap< storage::collection_t<typemap> >
        {
            typedef typemap type;
        };
        
        template<typename key_type, typename value_type, typename tail>
        struct container_collection_typemap< viennameta::typelist_t< viennameta::static_pair<key_type, value_type>, tail > >
        {
            typedef viennameta::typelist_t< viennameta::static_pair<key_type, value_type>, tail > type;
        };
        

        
        
        template<typename typemap>
        struct container_collection< storage::collection_t<typemap> >
        {
            typedef storage::collection_t<typemap> type;
        };
        
        
        template<typename domain_container_collection_type_, typename inserter_type_>
        struct container_collection< topology_t<domain_container_collection_type_, inserter_type_> >
        {
            typedef domain_container_collection_type_ type;
        };
        
        template<typename domain_type>
        struct element_typelist
        {
            typedef typename container_collection<domain_type>::type container_collection;
            typedef typename viennagrid::storage::container_collection::result_of::value_typelist<container_collection>::type type;
        };
        
        template<typename something, typename boundary_element_type>
        struct is_boundary_type;
        
        template<typename boundary_element_type>
        struct is_boundary_type< viennameta::null_type, boundary_element_type >
        {
            static const bool value = false;
        };
        
        template<typename head, typename tail, typename boundary_element_type>
        struct is_boundary_type< viennameta::typelist_t<head, tail>, boundary_element_type >
        {
            typedef typename head::first boundary_container_type;
            typedef typename boundary_container_type::value_type element_type;
            
            static const bool value = viennameta::EQUAL< element_type, boundary_element_type >::value ? true : is_boundary_type<tail, boundary_element_type>::value;
        };
        
        template<typename element_tag, typename bnd_cell_container_typelist_, typename id_tag, typename boundary_element_type>
        struct is_boundary_type< element_t<element_tag, bnd_cell_container_typelist_, id_tag>, boundary_element_type>
        {
            static const bool value = is_boundary_type<bnd_cell_container_typelist_, boundary_element_type>::value;
        };
        
        
        
        
        template<typename element_typelist, typename element_type>
        struct referencing_element_typelist_impl;

        template<typename element_type>
        struct referencing_element_typelist_impl<viennameta::null_type, element_type>
        {
            typedef viennameta::null_type type;
        };
        
        template<typename head, typename tail, typename element_type>
        struct referencing_element_typelist_impl< viennameta::typelist_t<head, tail>, element_type >
        {
            typedef typename referencing_element_typelist_impl<tail, element_type>::type tail_typelist;
            
            typedef typename viennameta::IF<
                is_boundary_type<head, element_type>::value,
                viennameta::typelist_t< head, tail_typelist>,
                tail_typelist
            >::type type;
        };

        
        template<typename domain_type, typename element_type_or_tag>
        struct referencing_element_typelist
        {
            typedef typename element<domain_type, element_type_or_tag>::type element_type;
            typedef typename element_typelist<domain_type>::type elements;
            typedef typename referencing_element_typelist_impl<elements, element_type>::type type;
        };
        
        
        
        
        template<typename element_typelist>
        struct element_to_tag;

        template<>
        struct element_to_tag<viennameta::null_type>
        {
            typedef viennameta::null_type type;
        };
        
        template<typename element_tag, typename bnd_cell_container_typelist_, typename id_tag, typename tail>
        struct element_to_tag< viennameta::typelist_t< element_t<element_tag, bnd_cell_container_typelist_, id_tag>, tail > >
        {
            typedef viennameta::typelist_t<
                element_tag,
                typename element_to_tag<tail>::type
            > type;
        };
        
        
        
        
        template<typename something>
        struct cell_dimension;

        template<>
        struct cell_dimension<viennameta::null_type>
        {
            static const int value = -1;
        };
        
        template<typename key_type, typename value_type, typename tail>
        struct cell_dimension< viennameta::typelist_t< viennameta::static_pair<key_type, value_type>, tail > >
        {
            typedef typename value_type::value_type element_type;
            static const int current_topologic_dim = viennagrid::result_of::topologic_dimension<element_type>::value;
            static const int tail_topologic_dim = cell_dimension<tail>::value;
            
            static const int value = (current_topologic_dim < tail_topologic_dim) ? tail_topologic_dim : current_topologic_dim;
        };
        
        template<typename typemap>
        struct cell_dimension< storage::collection_t<typemap> >
        {
            static const int value = cell_dimension<typemap>::value;
        };
        
        template<typename domain_container_collection_type_, typename inserter_type_>
        struct cell_dimension< topology_t<domain_container_collection_type_, inserter_type_> >
        {
            static const int value = cell_dimension<domain_container_collection_type_>::value;
        };
        
        
        
        
        
        
        template<typename element_typelist, typename container_typemap>
        struct filter_element_container;
        
        template<typename container_typemap>
        struct filter_element_container<viennameta::null_type, container_typemap>
        {
            typedef viennameta::null_type type;
        };
        
        template<typename element_type, typename tail, typename container_typemap>
        struct filter_element_container<viennameta::typelist_t< element_type, tail> , container_typemap>
        {
            typedef typename viennameta::typemap::result_of::find<container_typemap, element_type>::type result_type;
            
            typedef typename filter_element_container<tail, container_typemap>::type new_tail;
            
            typedef typename viennameta::IF<
                viennameta::EQUAL< result_type, viennameta::not_found >::value,
                new_tail,
                viennameta::typelist_t<
                    result_type,
                    new_tail
                >
            >::type type;
        };
        
        
        
        
        
        
        template<
            typename topology_type,
            typename element_typelist = 
                typename storage::container_collection::result_of::value_typelist<
                    typename container_collection<topology_type>::type
                >::type,
            typename container_config = 
                storage::default_container_config
            >
        struct topologic_view
        {
            typedef typename topology_type::container_collection_type container_collection_type;
            typedef typename topology_type::inserter_type domain_inserter_type;
            
            typedef typename filter_element_container<element_typelist, typename container_collection_type::typemap>::type view_container_collection_typemap;
            typedef typename viennagrid::storage::result_of::view_collection<view_container_collection_typemap, container_config>::type view_container_collection_type;
            
            typedef typename viennagrid::storage::result_of::recursive_inserter<view_container_collection_type, domain_inserter_type>::type view_inserter_type;
            
            typedef topology_t<view_container_collection_type, view_inserter_type> type; 
        };
    }
    
    
    template<typename container_collection_type, typename inserter_type>
    void init_domain( topology_t<container_collection_type, inserter_type> & domain )
    {
        typedef typename inserter_type::id_generator_type id_generator_type;
        domain.get_inserter() = inserter_type( domain.get_container_collection(), id_generator_type() );
    }
    
    template<typename topology_type>
    topology_type create_topology()
    {
        topology_type domain;
        init_domain(domain);
        return domain;
    }
    
    
    
    template<typename container_collection_type>
    class view_domain_setter
    {
    public:
        
        view_domain_setter(container_collection_type & domain_) : domain(domain_) {}
        
        template<typename container_type>
        void operator()( container_type & container )
        {
            typedef typename container_type::value_type value_type;
            container.set_base_container( viennagrid::storage::collection::get<value_type>(domain) );
        }
        
        
    private:
        container_collection_type & domain;
    };
    
    
    template<typename topologic_view_type, typename topology_type> 
    void init_view( topologic_view_type & view, topology_type & domain )
    {
        view_domain_setter< typename result_of::container_collection<topology_type>::type > functor(domain.get_container_collection());
        viennagrid::storage::collection::for_each(view.get_container_collection(), functor);
        
        view.get_inserter() = typename topologic_view_type::inserter_type( view.get_container_collection(), domain.get_inserter() );
    }
    

    template<typename view_type>
    struct create_view_helper;

    
    template<typename container_collection_type_, typename inserter_type_>
    struct create_view_helper< topology_t<container_collection_type_,inserter_type_> >
    {
        typedef topology_t<container_collection_type_,inserter_type_> topologic_view_type;
        
        template<typename domain_type>
        static topologic_view_type create( domain_type & domain )
        {
            topologic_view_type view;      
            init_view(view, domain);
            return view;
        }
    };
    
    template<typename topologic_view_type, typename topology_type>
    topologic_view_type create_view( topology_type & domain )
    {
        return create_view_helper<topologic_view_type>::create(domain);
    }
    
    template<typename container_collection_type>
    class handle_domain_functor
    {
    public:
        
        handle_domain_functor(container_collection_type & collection_) : collection(collection_) {}
        
        template<typename container_type>
        void operator()( container_type & container )
        {
            typedef typename container_type::value_type value_type;
            //container.set_base_container(  );
            storage::view::handle( viennagrid::storage::collection::get<value_type>(collection), container);
        }
        
        
    private:
        container_collection_type & collection;
    };
    
    template<typename view_type, typename domain_type>
    void handle_domain( view_type & view, domain_type & domain )
    {
        handle_domain_functor< typename result_of::container_collection<domain_type>::type > functor( container_collection(domain) );
        viennagrid::storage::collection::for_each( container_collection(view), functor);
    }
    
    
    
    
    
    template<typename view_type, typename domain_type, typename handle_type>
    void add_handle( view_type & view, domain_type & domain, handle_type handle )
    {
        typedef typename storage::handle::value_type<handle_type>::type value_type;
        value_type & element = dereference_handle(domain, handle);
        
        typedef typename viennagrid::result_of::element_range< view_type, value_type >::type range_type;
        typedef typename viennagrid::result_of::iterator<range_type>::type iterator_type;
        
        iterator_type it = find_by_id( view, element.id() );
        if ( it == elements<value_type>(view).end() )
            viennagrid::storage::collection::get<value_type>( container_collection(view) ).insert_handle( handle );
    }

    
    
    
    template<typename domain_container_collection_type_, typename inserter_type_>
    topology_t<domain_container_collection_type_, inserter_type_> & topology( topology_t<domain_container_collection_type_, inserter_type_> & domain) { return domain; }
    
    template<typename domain_container_collection_type_, typename inserter_type_>
    const topology_t<domain_container_collection_type_, inserter_type_> & topology( const topology_t<domain_container_collection_type_, inserter_type_> & domain) { return domain; }
    
    template<typename domain_type>
    typename result_of::container_collection< domain_type >::type & container_collection( domain_type & domain)
    { return topology(domain).get_container_collection(); }

    template<typename domain_type>
    const typename result_of::container_collection< domain_type >::type & container_collection( const domain_type & domain)
    { return topology(domain).get_container_collection(); }


    template<typename domain_type>
    typename result_of::topology<domain_type>::type::inserter_type & inserter(domain_type & domain)
    { return topology(domain).get_inserter(); }
    
    template<typename domain_type>
    typename result_of::topology<domain_type>::type::inserter_type const & inserter(domain_type const & domain)
    { return topology(domain).get_inserter(); }
    

    template<typename domain_type>
    typename result_of::topology<domain_type>::type::inserter_type::id_generator_type & id_generator(domain_type & domain)
    { return inserter(domain).get_id_generator(); }
    
    template<typename domain_type>
    typename result_of::topology<domain_type>::type::inserter_type::id_generator_type const & id_generator(domain_type const & domain)
    { return inserter(domain).get_id_generator(); }
    
    
    
    template<typename domain_type, typename handle_type>
    typename storage::handle::value_type<handle_type>::type & dereference_handle( domain_type & domain, const handle_type & handle)
    {
        typedef typename storage::handle::value_type<handle_type>::type value_type;
        return storage::collection::get<value_type>(container_collection(domain)).dereference_handle( handle );
    }
    
    template<typename domain_type, typename handle_type>
    const typename storage::handle::value_type<handle_type>::type & dereference_handle( const domain_type & domain, const handle_type & handle)
    {
        typedef typename storage::handle::value_type<handle_type>::type value_type;
        return storage::collection::get<value_type>(container_collection(domain)).dereference_handle( handle );
    }
    
    
    
  template<typename domain_type,
           typename element_tag_1, typename bnd_cell_typelist_1, typename id_type_1,
           typename handle_type_2>
  typename viennagrid::result_of::element<domain_type, viennagrid::vertex_tag>::type
  local_vertex(const domain_type & domain,
               element_t<element_tag_1, bnd_cell_typelist_1, id_type_1> const & host_ncell,
               handle_type_2 const & bnd_kcell_handle,
               std::size_t index)
  {
    typedef typename viennagrid::storage::handle::value_type<handle_type_2>::type element_type_2;
    const element_type_2 & bnd_kcell = viennagrid::dereference_handle(domain, bnd_kcell_handle);
    return viennagrid::elements< viennagrid::vertex_tag >(bnd_kcell)[host_ncell.global_to_local_orientation(bnd_kcell_handle, index)];
  }
    
    
    
    template<typename container_type>
    class dereference_handle_comperator
    {
    public:
        
        dereference_handle_comperator(const container_type & container_) : container(container_) {}
        
        template<typename handle>
        bool operator() ( handle h1, handle h2 )
        {
            return &viennagrid::dereference_handle( container, h1 ) < &viennagrid::dereference_handle( container, h2 );
        }
        
    private:
        const container_type & container;
    };
    
}



namespace viennagrid
{
    
    namespace result_of
    {
        
        template<typename domain_container_collection_type_, typename inserter_type_, typename element_type_or_tag>
        struct element< topology_t<domain_container_collection_type_, inserter_type_>, element_type_or_tag >
        {
            typedef typename element<domain_container_collection_type_, element_type_or_tag>::type type;
        };
        
        template<typename domain_container_collection_type_, typename inserter_type_, typename element_type_or_tag>
        struct handle<topology_t<domain_container_collection_type_, inserter_type_>, element_type_or_tag>
        {
            typedef typename handle<domain_container_collection_type_, element_type_or_tag>::type type;
        };
        
        template<typename domain_container_collection_type_, typename inserter_type_, typename element_type_or_tag>
        struct const_handle<topology_t<domain_container_collection_type_, inserter_type_>, element_type_or_tag>
        {
            typedef typename const_handle<domain_container_collection_type_, element_type_or_tag>::type type;
        };
        
        
        template<typename domain_container_collection_type_, typename inserter_type_, typename element_type_or_tag>
        struct element_range< topology_t<domain_container_collection_type_, inserter_type_>, element_type_or_tag >
        {
            typedef typename element_range<domain_container_collection_type_, element_type_or_tag>::type type;
        };
        
        template<typename domain_container_collection_type_, typename inserter_type_, typename element_type_or_tag>
        struct const_element_range<topology_t<domain_container_collection_type_, inserter_type_>, element_type_or_tag>
        {
            typedef typename const_element_range<domain_container_collection_type_, element_type_or_tag>::type type;
        };
        
        
        
        

        
        
        
        
        template<typename typemap>
        struct topologic_cell_dimension_impl;
        
        template<>
        struct topologic_cell_dimension_impl<viennameta::null_type>
        {
            static const int value = -1;
        };
        
        template<typename element_type, typename element_container_type, typename tail>
        struct topologic_cell_dimension_impl< viennameta::typelist_t< viennameta::static_pair<element_type, element_container_type>, tail > >
        {
            static const int tail_cell_dimension = topologic_cell_dimension_impl<tail>::value;
            static const int current_element_dimension = topologic_dimension<element_type>::value;
            
            static const int value = (tail_cell_dimension > current_element_dimension) ? tail_cell_dimension : current_element_dimension;
        };
        
        template<typename something>
        struct topologic_cell_dimension
        {
            static const int value = topologic_cell_dimension_impl< typename container_collection_typemap<something>::type >::value;
        };
        
        
        

        
        template<typename typemap, int topologic_dimension>
        struct elements_of_topologic_dim_impl;
        
        template<int topologic_dimension>
        struct elements_of_topologic_dim_impl< viennameta::null_type, topologic_dimension >
        {
            typedef viennameta::null_type type;
        };
        
        template<typename element_type, typename element_container_type, typename tail, int topologic_dimension>
        struct elements_of_topologic_dim_impl< viennameta::typelist_t< viennameta::static_pair<element_type, element_container_type>, tail >, topologic_dimension >
        {
            typedef typename elements_of_topologic_dim_impl<tail, topologic_dimension>::type tail_typelist;
            
            typedef typename viennameta::IF<
                viennagrid::result_of::topologic_dimension<element_type>::value == topologic_dimension,
                typename viennameta::typelist::result_of::push_back<tail_typelist, element_type>::type,
                tail_typelist
            >::type type;
        };
        
        template<typename something, int topologic_dimension>
        struct elements_of_topologic_dim
        {
            typedef typename elements_of_topologic_dim_impl< typename container_collection_typemap<something>::type, topologic_dimension>::type type;
        };
        
        
        
        template<typename something>
        struct cell_types
        {
            typedef typename elements_of_topologic_dim<
                something,
                topologic_cell_dimension<something>::value
            >::type type;
        };

        
        template<typename something>
        struct cell_type
        {
            typedef typename cell_types<something>::type all_cell_types;
            typedef typename viennameta::STATIC_ASSERT< viennameta::typelist::result_of::size<all_cell_types>::value == 1 >::type static_assert_typedef;
            
            typedef typename viennameta::typelist::result_of::at<all_cell_types,0>::type type;
        };
    }
    
    
    
    template<typename domain_type, typename functor>
    struct for_each_element_functor
    {
        for_each_element_functor( domain_type & domain_, functor f_ ) : domain(domain_), f(f_) {}
        
        template<typename element_type>
        void operator() ( viennameta::tag<element_type> )
        {
            typedef typename viennagrid::result_of::element_range<domain_type, element_type>::type element_range_type;
            typedef typename viennagrid::result_of::iterator<element_range_type>::type element_range_iterator;
            
            element_range_type range = viennagrid::elements(domain);
            for (element_range_iterator it = range.begin(); it != range.end(); ++it)
                f(*it);
        }
        
        template<typename element_type>
        void operator() ( viennameta::tag<element_type> ) const
        {
            typedef typename viennagrid::result_of::const_element_range<domain_type, element_type>::type element_range_type;
            typedef typename viennagrid::result_of::iterator<element_range_type>::type element_range_iterator;
            
            element_range_type range = viennagrid::elements(domain);
            for (element_range_iterator it = range.begin(); it != range.end(); ++it)
                f(*it);
        }
        
        functor f;
        domain_type & domain;
    };
    
    
    
    template<int topologic_dimension, typename domain_type, typename functor>
    void for_each( domain_type & domain, functor f )
    {
        for_each_element_functor<domain_type, functor> for_each_functor(domain, f);
        typedef typename viennagrid::result_of::elements_of_topologic_dim<domain_type, topologic_dimension>::type element_typelist;
        
        viennameta::typelist::for_each<element_typelist>( for_each_functor );
    }
    
    template<int topologic_dimension, typename domain_type, typename functor>
    void for_each( domain_type const & domain, functor f )
    {
        for_each_element_functor<const domain_type, functor> for_each_functor(domain, f);
        typedef typename viennagrid::result_of::elements_of_topologic_dim<domain_type, topologic_dimension>::type element_typelist;
        
        viennameta::typelist::for_each<element_typelist>( for_each_functor );
    }
    
    
    
    
    
    template<typename element_type_or_tag, typename domain_container_collection_type_, typename inserter_type_>
    typename result_of::element_range<topology_t<domain_container_collection_type_, inserter_type_>, element_type_or_tag>::type elements(topology_t<domain_container_collection_type_, inserter_type_> & domain)
    {
        return elements<element_type_or_tag>(domain.get_container_collection());
    }
    
    template<typename element_type_or_tag, typename domain_container_collection_type_, typename inserter_type_>
    typename result_of::const_element_range<topology_t<domain_container_collection_type_, inserter_type_>, element_type_or_tag>::type elements(const topology_t<domain_container_collection_type_, inserter_type_> & domain)
    {
        return elements<element_type_or_tag>(domain.get_container_collection());
    }

    
    
//     template<typename domain_type, typename id_type>
//     bool element_present(const domain_type & domain, id_type id)
//     {
//         typedef typename id_type::value_type element_type;
//         typedef typename element_type::tag element_tag;
//         typedef typename viennagrid::result_of::const_element_range<domain_type, element_tag>::type RangeType;
//         typedef typename viennagrid::result_of::const_handle_iterator<RangeType>::type RangeIterator;
//         
//         RangeType range = viennagrid::elements<element_tag>(domain);
//         for (RangeIterator it = range.handle_begin(); it != range.handle_end(); ++it)
//         {
//             if ( viennagrid::dereference_handle(domain, *it).id() == id )
//                 return true;
//         }
//         
//         return false;
//     }
//     




    template<typename element_type_or_tag, typename domain_type>
    typename viennagrid::result_of::id_type< typename viennagrid::result_of::element<domain_type, element_type_or_tag>::type >::type id_upper_bound( domain_type const & domain )
    {
        typedef typename viennagrid::result_of::element<domain_type, element_type_or_tag>::type element_type;
        return id_generator(domain).max_id( viennameta::tag<element_type>() );
    }
    
    
    
    template<typename domain_type, typename id_type>
    typename viennagrid::result_of::iterator< typename viennagrid::result_of::element_range<domain_type, typename id_type::value_type::tag>::type >::type
            find(domain_type & domain, id_type id )
    {
        typedef typename id_type::value_type element_type;
        typedef typename element_type::tag element_tag;
        typedef typename viennagrid::result_of::element_range<domain_type, element_tag>::type RangeType;
        RangeType range = viennagrid::elements<element_tag>(domain);
        return std::find_if(
                    range.begin(),
                    range.end(),
                    viennagrid::storage::id_compare<id_type>(id)
            );
    }
    
    template<typename domain_type, typename id_type>
    typename viennagrid::result_of::const_iterator< typename viennagrid::result_of::element_range<domain_type, typename id_type::value_type::tag>::type >::type
            find(const domain_type & domain, id_type id )
    {
        typedef typename id_type::value_type element_type;
        typedef typename element_type::tag element_tag;
        typedef typename viennagrid::result_of::const_element_range<domain_type, element_tag>::type RangeType;
        RangeType range = viennagrid::elements<element_tag>(domain);
        return std::find_if(
                    range.begin(),
                    range.end(),       
                    viennagrid::storage::id_compare<id_type>(id)
            );
    }

    

    
    
    template<bool generate_id, bool call_callback, typename domain_type, typename element_tag, typename boundary_cell_container_typelist, typename id_type>
    std::pair<
                typename viennagrid::storage::result_of::container_of<
                    typename result_of::container_collection<domain_type>::type,
                    viennagrid::element_t<element_tag, boundary_cell_container_typelist, id_type>
                >::type::handle_type,
                bool
            >
        push_element( domain_type & domain, const viennagrid::element_t<element_tag, boundary_cell_container_typelist, id_type> & element)
    {
        return inserter(domain).template insert<generate_id, call_callback>(element);
    }
    
    
    template<typename domain_type, typename element_tag, typename boundary_cell_container_typelist, typename id_type>
    std::pair<
                typename viennagrid::storage::result_of::container_of<
                    typename result_of::container_collection<domain_type>::type,
                    viennagrid::element_t<element_tag, boundary_cell_container_typelist, id_type>
                >::type::handle_type,
                bool
            >
        push_element( domain_type & domain, const viennagrid::element_t<element_tag, boundary_cell_container_typelist, id_type> & element)
    {
        return inserter(domain)(element);
    }
    
    template<typename domain_type, typename element_type>
    std::pair<
                typename viennagrid::storage::result_of::container_of<
                    typename result_of::container_collection<domain_type>::type,
                    element_type
                >::type::handle_type,
                bool
            >
        push_element_noid( domain_type & domain, const element_type & element)
    {
        return inserter(domain).template insert<false, true>(element);
    }

    

    
        
    
    template<typename element_tag, typename boundary_cell_container_typelist, typename id_type>
    void set_vertex(
            viennagrid::element_t<element_tag, boundary_cell_container_typelist, id_type> & element,
            typename viennagrid::result_of::handle< viennagrid::element_t<element_tag, boundary_cell_container_typelist, id_type>, vertex_tag >::type vertex_handle,
            unsigned int pos
        )
    {
        element.container( viennagrid::dimension_tag<0>() ).set_handle( vertex_handle, pos );
    }
    
    template<typename domain_type>
    typename result_of::handle<domain_type, viennagrid::vertex_tag>::type get_vertex_handle( domain_type & domain, unsigned int pos )
    {
        typedef typename result_of::element<domain_type, viennagrid::vertex_tag>::type vertex_type;
        return viennagrid::storage::collection::get<vertex_type>( container_collection(domain) ).handle_at(pos);
    }
    

}



#endif
