/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaGrid - The Vienna Grid Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    MIT (X11), see file LICENSE in the ViennaGrid base directory
======================================================================= */

#ifndef VIENNAGRID_IO_NETGEN_READER_GUARD
#define VIENNAGRID_IO_NETGEN_READER_GUARD


#include <fstream>
#include <iostream>
#include "viennagrid/forwards.h"

namespace viennagrid
{
  namespace io
  {

    struct netgen_reader
    {
      template <typename DomainType>
      int operator()(DomainType & domain, std::string const & filename) const
      {
        typedef typename DomainType::config_type                     DomainConfiguration;

        typedef typename DomainConfiguration::numeric_type                 CoordType;
        typedef typename DomainConfiguration::dimension_tag              DimensionTag;
        typedef typename DomainConfiguration::cell_tag                   CellTag;

        typedef typename result_of::point_type<DomainConfiguration>::type                              PointType;
        typedef typename result_of::ncell_type<DomainConfiguration, 0>::type                           VertexType;
        typedef typename result_of::ncell_type<DomainConfiguration, CellTag::topology_level>::type     CellType;
        //typedef typename DomainTypes<DomainConfiguration>::segment_type  Segment;

        typedef typename viennagrid::result_of::ncell_container<DomainType, 0>::type   VertexContainer;
        typedef typename viennagrid::result_of::iterator<VertexContainer>::type        VertexIterator;
            
        typedef typename viennagrid::result_of::ncell_container<DomainType, 1>::type   EdgeContainer;
        typedef typename viennagrid::result_of::iterator<EdgeContainer>::type          EdgeIterator;

        typedef typename viennagrid::result_of::ncell_container<DomainType, CellTag::topology_level-1>::type   FacetContainer;
        typedef typename viennagrid::result_of::iterator<FacetContainer>::type                                 FacetIterator;

        typedef typename viennagrid::result_of::ncell_container<DomainType, CellTag::topology_level>::type     CellContainer;
        typedef typename viennagrid::result_of::iterator<CellContainer>::type                                  CellIterator;

        std::ifstream reader(filename.c_str());

        //Segment & segment = *(domain.begin());

        std::cout << "* netgen_reader::operator(): Reading file " << filename << std::endl;

        if (!reader){
          std::cerr << "Cannot open file " << filename << std::endl;
          throw "Cannot open file";
          return EXIT_FAILURE;
        }

        try{
          std::string token;
          long node_num = 0;
          long cell_num = 0;
      
          
          //
          // Read vertices:
          //
          reader >> node_num;

          //std::cout << "Reading " << node_num << " vertices... " << std::endl;  
          //reserve the memory:
          domain.reserve_vertices(node_num);
          VertexType vertex;
      
          for (int i=0; i<node_num; i++)
          {
      
            for (int j=0; j<DimensionTag::value; j++)
              reader >> vertex.getPoint()[j];
      
            vertex.setID(i);
            domain.add(vertex);
          }
      
          //std::cout << "DONE" << std::endl;
      
          //
          // Read cells:
          //
          reader >> cell_num;
          domain.reserve_cells(cell_num);
          CellType cell;
          //std::cout << "Filling cells:" << std::endl;
      
          for (int i=0; i<cell_num; ++i)
          {
            long vertex_num;
            VertexType *vertices[traits::subcell_desc<CellTag, 0>::num_elements];

            long dummy;
            reader >> dummy; //the first value is some number i can't make use of...
      
            for (int j=0; j<traits::subcell_desc<CellTag, 0>::num_elements; ++j)
            {
              reader >> vertex_num;
              vertices[j] = &(domain.vertex(vertex_num - 1));  //Note that Netgen uses vertex indices with base 1
            }
      
            //std::cout << std::endl << "Adding cell: " << &cell << " at " << cell_id << std::endl;
            cell.setVertices(&(vertices[0]));
            cell.setID(i);
            domain.add(cell);
      
            //progress info:
            if (i % 50000 == 0 && i > 0)
              std::cout << "* netgen_reader::operator(): " << i << " out of " << cell_num << " cells read." << std::endl;
          }
      
      
        } catch (...) {
          std::cerr << "Problems while reading file " << filename << std::endl;
        }
      
        //std::cout << "File-Reader finished!" << std::endl;  
        
        return EXIT_SUCCESS;
      } //operator()
      
    }; //class netgen_reader
  
  } //namespace io
} //namespace viennagrid

#endif
