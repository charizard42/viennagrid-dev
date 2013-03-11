#ifndef VIENNAGRID_IO_POLY_READER_GUARD
#define VIENNAGRID_IO_POLY_READER_GUARD

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


#include <fstream>
#include <iostream>
#include <assert.h>

#include "viennadata/api.hpp"

#include "viennagrid/forwards.hpp"
#include "viennagrid/io/helper.hpp"

#include "viennagrid/domain/geometric_domain.hpp"

/** @file netgen_reader.hpp
    @brief Provides a reader for Netgen files
*/



namespace viennagrid
{
  namespace io
  {
    struct poly_attribute_tag {};
  }
}

VIENNADATA_ENABLE_TYPE_BASED_KEY_DISPATCH(viennagrid::io::poly_attribute_tag)




namespace viennagrid
{
  namespace io
  {
      
      template<typename stream_type>
      bool get_valid_line( stream_type & stream, std::string & line, char comment_line = '#' )
      {
          std::string tmp;
          
          while (true)
          {
                if (!std::getline(stream, tmp))
                    return false;
                
                std::size_t pos = tmp.find(comment_line);
                if (pos != std::string::npos)
                    tmp = tmp.substr(0, pos);
                
                for (std::size_t i = 0; i != tmp.size(); ++i)
                {
                    if ( tmp[i] != ' ' )
                    {
                        line = tmp.substr(i, std::string::npos);
                        return true;
                    }
                }
          }
      }
      
      
      



    /** @brief Reader for Netgen files obtained from the 'Export mesh...' menu item. Tested with Netgen version 4.9.12. */
    struct poly_reader
    {
      /** @brief The functor interface triggering the read operation.
       * 
       * @param domain    A ViennaGrid domain
       * @param filename  Name of the file
       */
      template <typename GeometricDomainType>
      int operator()(GeometricDomainType & domain, std::string const & filename) const
      {
        //typedef typename DomainType::config_type                         ConfigType;
        
//         typedef typename GeometricSegmentContainerType::value_type SegmentType;
        
        typedef typename viennagrid::result_of::point_type<GeometricDomainType>::type    PointType;
        typedef typename viennagrid::result_of::coord_type< PointType >::type         CoordType;

        //typedef typename ConfigType::numeric_type                        CoordType;
        //typedef typename ConfigType::coordinate_system_tag               CoordinateSystemTag;
        enum { point_dim = viennagrid::traits::static_size<PointType>::value };
        
        typedef plc_tag CellTag;
        typedef typename result_of::element<GeometricDomainType, CellTag>::type CellType;
        //typedef typename CellType::tag                            CellTag;
        typedef typename result_of::element_hook<GeometricDomainType, CellTag>::type                           CellHookType;

        //typedef typename result_of::point<ConfigType>::type                              PointType;
        typedef typename result_of::element<GeometricDomainType, vertex_tag>::type                           VertexType;
        typedef typename result_of::element_hook<GeometricDomainType, vertex_tag>::type                           VertexHookType;
        typedef typename VertexType::id_type VertexIDType;
        //typedef typename result_of::ncell<DomainType, CellTag::dim>::type     CellType;
        
        typedef typename result_of::element<GeometricDomainType, polygon_tag>::type                           PolygonType;
        typedef typename result_of::element_hook<GeometricDomainType, polygon_tag>::type                           PolygonHookType;
        
        typedef typename result_of::element<GeometricDomainType, line_tag>::type                           LineType;
        typedef typename result_of::element_hook<GeometricDomainType, line_tag>::type                           LineHookType;

//         typedef typename viennagrid::result_of::element_range<GeometricDomainType, vertex_tag>::type   VertexRange;
//         typedef typename viennagrid::result_of::iterator<VertexRange>::type        VertexIterator;
//             
//         typedef typename viennagrid::result_of::element_range<GeometricDomainType, line_tag>::type   EdgeRange;
//         typedef typename viennagrid::result_of::iterator<EdgeRange>::type          EdgeIterator;
// 
//         typedef typename viennagrid::result_of::element_range<GeometricDomainType, typename CellType::tag::facet_tag >::type   FacetRange;
//         typedef typename viennagrid::result_of::iterator<FacetRange>::type                                 FacetIterator;
// 
//         typedef typename viennagrid::result_of::element_range<GeometricDomainType, CellTag>::type     CellRange;
//         typedef typename viennagrid::result_of::iterator<CellRange>::type                                  CellIterator;
        
        std::ifstream reader(filename.c_str());
        
        #if defined VIENNAGRID_DEBUG_STATUS || defined VIENNAGRID_DEBUG_IO
        std::cout << "* netgen_reader::operator(): Reading file " << filename << std::endl;
        #endif

        if (!reader)
        {
          throw cannot_open_file_exception(filename);
          return EXIT_FAILURE;
        }

        std::string tmp;
        std::istringstream current_line;
        
        long node_num = 0;
        long dim = 0;
        long attribute_num = 0;
        long boundary_marker_num = 0;
        
        
       
      
        if (!reader.good())
          throw bad_file_format_exception(filename, "File is empty.");
        
        //
        // Read vertices:
        //
        if (!get_valid_line(reader, tmp))
            throw bad_file_format_exception(filename, "EOF encountered when reading information");
          
        
        current_line.str(tmp); current_line.clear();
        current_line >> node_num >> dim >> attribute_num >> boundary_marker_num;
        
        if (node_num < 0)
            throw bad_file_format_exception(filename, "POLY file has less than 0 nodes");
        if (dim != 3)
            throw bad_file_format_exception(filename, "POLY dimension is not 3");
        if (attribute_num < 0)
            throw bad_file_format_exception(filename, "POLY file has less than 0 attributes");
        if ((boundary_marker_num < 0) || (boundary_marker_num > 1))
            throw bad_file_format_exception(filename, "POLY file has not 0 or 1 boundary marker");
        
        #if defined VIENNAGRID_DEBUG_STATUS || defined VIENNAGRID_DEBUG_IO
        std::cout << "* netgen_reader::operator(): Reading " << node_num << " vertices... " << std::endl;  
        #endif

        for (int i=0; i<node_num; i++)
        {
            if (!get_valid_line(reader, tmp))
                throw bad_file_format_exception(filename, "EOF encountered when reading information");
            
            typename VertexIDType::base_id_type id;
            
            current_line.str(tmp); current_line.clear();
            current_line >> id;
          
            VertexHookType vertex = viennagrid::create_element<VertexType>( domain, VertexIDType(id) );
            PointType & p = viennagrid::point( domain, vertex );
          
            for (int j=0; j<point_dim; j++)
                current_line >> p[j];
            
            std::vector<CoordType> attributes(attribute_num);
            for (int j=0; j<attribute_num; j++)
                current_line >> attributes[j];
            
            viennadata::access<poly_attribute_tag, std::vector<CoordType> >()(viennagrid::dereference_hook(domain, vertex)) = attributes;
        }
    
        //std::cout << "DONE" << std::endl;
        if (!reader.good())
          throw bad_file_format_exception(filename, "EOF encountered when reading number of cells.");
          
        
        //
        // Read facets:
        //
        long facet_num = 0;
        if (!get_valid_line(reader, tmp))
            throw bad_file_format_exception(filename, "EOF encountered when reading information");
          
        
        current_line.str(tmp); current_line.clear();
        current_line >> facet_num >> boundary_marker_num;
        
        if (facet_num < 0)
            throw bad_file_format_exception(filename, "POLY file has less than 0 facets");
        if ((boundary_marker_num < 0) || (boundary_marker_num > 1))
            throw bad_file_format_exception(filename, "POLY file has not 0 or 1 boundary marker");

        //domain.reserve_cells(cell_num);
        
        #if defined VIENNAGRID_DEBUG_STATUS || defined VIENNAGRID_DEBUG_IO
        std::cout << "* netgen_reader::operator(): Reading " << cell_num << " cells... " << std::endl;  
        #endif
        
        //CellHookType cell = viennagrid::create_element<CellType>(domain);
        //std::cout << "Filling " << cell_num << " cells:" << std::endl;
    
        for (int i=0; i<facet_num; ++i)
        {
            long polygon_num;
            long hole_num;
          
            if (!get_valid_line(reader, tmp))
                throw bad_file_format_exception(filename, "EOF encountered when reading information");
            
            
            current_line.str(tmp); current_line.clear();
            current_line >> polygon_num >> hole_num;
            
            if (polygon_num < 0)
                throw bad_file_format_exception(filename, "POLY facet has less than 0 polygons");
            if (hole_num < 0)
                throw bad_file_format_exception(filename, "POLY facet has less than 0 holes");
            
            
            std::vector<PolygonHookType> polygons;
            std::vector<LineHookType> lines;
            std::vector<VertexHookType> vertices;
            
            
            for (int j = 0; j<polygon_num; ++j)
            {
                long vertex_num;
                
                if (!get_valid_line(reader, tmp))
                    throw bad_file_format_exception(filename, "EOF encountered when reading information");
                
                
                current_line.str(tmp); current_line.clear();
                current_line >> vertex_num;
                
                if (vertex_num < 0)
                    throw bad_file_format_exception(filename, "POLY polygon has less than 0 vertices");
                
                std::vector<VertexHookType> vertex_hooks(vertex_num);
                
                for (int k = 0; k<vertex_num; ++k)
                {
                    long id;
                    current_line >> id;
                    vertex_hooks[k] = *viennagrid::find_hook( domain, VertexIDType(id) );
                }
                
                if (vertex_num == 1)
                {
                    vertices.push_back( vertex_hooks.front() );
//                     std::cout << " Added loose vertex" << std::endl;
                }
                else if (vertex_num == 2)
                {
                    lines.push_back( viennagrid::create_element<LineType>(domain, vertex_hooks.begin(), vertex_hooks.end()) );
//                     std::cout << " Added loose line" << std::endl;
                }
                else
                {
                    polygons.push_back( viennagrid::create_element<PolygonType>(domain, vertex_hooks.begin(), vertex_hooks.end()) );
//                     std::cout << " Added polygon " << vertex_hooks.size() << std::endl;
                }
            }
            
            
            viennagrid::create_element<CellType>(
                domain,
                polygons.begin(), polygons.end(),
                polygons.begin(), polygons.begin(),
                lines.begin(), lines.end(),
                vertices.begin(), vertices.end()
            );
            
//             std::cout << "Created PLC " << i << std::endl;
        }
        //std::cout << "All done!" << std::endl;
        
        return EXIT_SUCCESS;
      } //operator()
      
    }; //class netgen_reader
  
  } //namespace io
} //namespace viennagrid

#endif