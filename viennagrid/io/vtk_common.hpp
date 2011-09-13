#ifndef VIENNAGRID_IO_VTK_COMMON_HPP
#define VIENNAGRID_IO_VTK_COMMON_HPP

/* =======================================================================
   Copyright (c) 2011, Institute for Microelectronics,
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


#include <fstream>
#include <iostream>
#include "viennagrid/io/helper.hpp"
#include "viennagrid/topology/all.hpp"
#include "viennagrid/forwards.h"

namespace viennagrid
{
  namespace io
  {
    // translates element tags to VTK type identifiers
    // see http://www.vtk.org/VTK/img/file-formats.pdf, Figure 2, for an overview
    template <typename ElementTag>
    struct ELEMENT_TAG_TO_VTK_TYPE
    {
      enum{ ReturnValue = ElementTag::ERROR_ELEMENT_TYPE_NOT_SUPPORTED };
    };

    template <>
    struct ELEMENT_TAG_TO_VTK_TYPE<hexahedron_tag>
    {
      enum{ value = 12 };
    };

    template <>
    struct ELEMENT_TAG_TO_VTK_TYPE<tetrahedron_tag>
    {
      enum{ value = 10 };
    };
    
    template <>
    struct ELEMENT_TAG_TO_VTK_TYPE<quadrilateral_tag>
    {
      enum{ value = 9 };
    };

    template <>
    struct ELEMENT_TAG_TO_VTK_TYPE<triangle_tag>
    {
      enum{ value = 5 };
    };

    template <>
    struct ELEMENT_TAG_TO_VTK_TYPE<hypercube_tag<1> >
    {
      enum{ value = 3 };
    };
    
    template <>
    struct ELEMENT_TAG_TO_VTK_TYPE<simplex_tag<1> >
    {
      enum{ value = 3 };
    };
    
    //
    // Tranformations of reference orientations:
    //
    template <typename CellTag>
    struct vtk_to_viennagrid_orientations
    {
      long operator()(long j) const { return j; }  //default case: do nothing
    };

    template <typename CellTag>
    struct viennagrid_to_vtk_orientations
    {
      long operator()(long j) const { return j; }  //default case: do nothing
    };

    // for quadrilaterals:
    template <>
    struct vtk_to_viennagrid_orientations<quadrilateral_tag>
    {
      long operator()(long j) const
      {
        switch (j)
        {
          case 2: return 3;
          case 3: return 2;
          default: return j;
        }
        return j;
      }
    };
    
    template <>
    struct viennagrid_to_vtk_orientations<quadrilateral_tag>
    {
      long operator()(long j) const
      {
        return vtk_to_viennagrid_orientations<quadrilateral_tag>()(j);
      }
    };

    // for hexahedra:
    template <>
    struct vtk_to_viennagrid_orientations<hexahedron_tag>
    {
      long operator()(long j) const
      {
        switch (j)
        {
          case 2: return 3;
          case 3: return 2;
          case 6: return 7;
          case 7: return 6;
          default: return j;
        }
        return j;
      }
    };

    template <>
    struct viennagrid_to_vtk_orientations<hexahedron_tag>
    {
      long operator()(long j) const
      {
        return vtk_to_viennagrid_orientations<hexahedron_tag>()(j);
      }
    };






    template <typename DomainSegmentType,
              typename IDHandler = typename viennagrid::result_of::element_id_handler<typename DomainSegmentType::config_type,
                                                                                      viennagrid::point_tag>::type
             >
    class vtk_vertex_id_repository //default case: This is a segment
    {
        typedef typename DomainSegmentType::config_type                      ConfigType;
        typedef typename viennagrid::result_of::segment<ConfigType>::type    SegmentType;
        typedef typename viennagrid::result_of::ncell<ConfigType, 0>::type   VertexType;
        
      public:
        vtk_vertex_id_repository(SegmentType const & seg) {}
      
        long operator()(VertexType const & v, SegmentType const & seg) const
        {
          return viennadata::access<segment_mapping_key<SegmentType>, long>(seg)(v);
        }
    };
    
    // The simple case: Vertices know their global IDs:
    template <typename ConfigType>
    class vtk_vertex_id_repository< viennagrid::domain_t<ConfigType>, viennagrid::integral_id >
    {
        typedef typename viennagrid::result_of::domain<ConfigType>::type     DomainType;
        typedef typename viennagrid::result_of::ncell<ConfigType, 0>::type   VertexType;
        
      public:
        vtk_vertex_id_repository(DomainType const & seg) {}
      
        long operator()(VertexType const & v, DomainType const & seg) const
        {
          return v.id();
        }
    };

    // The tough case: Vertices don't know their global IDs. Set up a map first:
    template <typename ConfigType>
    class vtk_vertex_id_repository< viennagrid::domain_t<ConfigType>, viennagrid::pointer_id >
    {
        typedef typename viennagrid::domain_t<ConfigType>                    DomainType;
        typedef typename viennagrid::result_of::ncell<ConfigType, 0>::type   VertexType;
        
      public:
        vtk_vertex_id_repository(DomainType const & domain)
        {
          typedef typename viennagrid::result_of::const_ncell_range<DomainType, 0>::type    VertexRange;
          typedef typename viennagrid::result_of::iterator<VertexRange>::type               VertexIterator;
          
          VertexRange vertices = viennagrid::ncells(domain);
          long vertex_index = 0;
          for (VertexIterator vit = vertices.begin();
                              vit != vertices.end();
                            ++vit, ++vertex_index)
          {
            ptr_to_global_id[&(*vit)] = vertex_index;
          }
        }
      
        long operator()(VertexType const & v, DomainType const &)
        {
          return ptr_to_global_id[&v];
        }
        
      private:
        std::map<VertexType const *, long>  ptr_to_global_id;
    };
    

    
  } //namespace io
} //namespace viennagrid

#endif

