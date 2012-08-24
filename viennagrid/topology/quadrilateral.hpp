#ifndef VIENNAGRID_TOPOLOGY_QUADRILATERAL_HPP
#define VIENNAGRID_TOPOLOGY_QUADRILATERAL_HPP

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

#include "viennagrid/forwards.h"
#include "viennagrid/topology/point.hpp"
#include "viennagrid/topology/line.hpp"

/** @file quadrilateral.hpp
    @brief Provides the topological definition of a quadrilateral
*/

namespace viennagrid
{
  /** @brief Topological description of a quadrilateral.*/
  template <>
  struct hypercube_tag<2>
  {
    enum{ dim = 2 };
    static std::string name() { return "Quadrilateral"; }
  };
  
  namespace topology
  {
  
    /** @brief Topological description of the boundary 0-cells of a quadrilateral */
    template <>
    struct bndcells<quadrilateral_tag, 0>
    {
      typedef point_tag             tag;

      typedef static_layout_tag     layout_tag;
      enum{ num = 4 };     //4 vertices
    };

    /** @brief Topological description of the boundary 1-cells of a quadrilateral */
    template <>
    struct bndcells<quadrilateral_tag, 1>
    {
      typedef hypercube_tag<1>       tag;

      typedef static_layout_tag     layout_tag;
      enum{ num = 4 };     //4 edges
    };



    //////// Parallelogram ////////
    /** @brief Fills a segment or a domain with the edges/facets of a quadrilateral */
    template <>
    struct bndcell_filler<quadrilateral_tag, 1>
    {
      //fill edges:
      //
      // Reference orientation: (v... vertices, e...edges)
      //        e3
      // v2 --------- v3
      //    |       |
      // e1 |       | e2
      //    |       |
      // v0 --------- v1
      //        e0
      
      
      template <typename ElementContainerType, typename VertexContainerType, typename OrientatationContainerType, typename Segment>
      static void fill(ElementContainerType & elements, const VertexContainerType & vertices, OrientatationContainerType & orientations, Segment & seg)
      {
        typename VertexContainerType::value_type edgevertices[2];
        typename utils::remove_pointer<typename ElementContainerType::value_type>::type edge;
        
        edgevertices[0] = vertices[0];
        edgevertices[1] = vertices[1];
        edge.vertices(edgevertices);
        elements[0] = seg.push_back(edge, &orientations[0]);

        edgevertices[0] = vertices[0];
        edgevertices[1] = vertices[2];
        edge.vertices(edgevertices);
        elements[1] = seg.push_back(edge, &orientations[1]);

        edgevertices[0] = vertices[1];
        edgevertices[1] = vertices[3];
        edge.vertices(edgevertices);
        elements[2] = seg.push_back(edge, &orientations[2]);

        edgevertices[0] = vertices[2];
        edgevertices[1] = vertices[3];
        edge.vertices(edgevertices);
        elements[3] = seg.push_back(edge, &orientations[3]);

    }
      
      template <typename ElementContainerType, typename VertexContainerType, typename Segment>
      static void fill(ElementContainerType & elements, const VertexContainerType & vertices, Segment & seg)
      {
        typename VertexContainerType::value_type edgevertices[2];
        typename utils::remove_pointer<typename ElementContainerType::value_type>::type edge;
        
        edgevertices[0] = vertices[0];
        edgevertices[1] = vertices[1];
        edge.vertices(edgevertices);
        elements[0] = seg.push_back(edge, NULL);

        edgevertices[0] = vertices[0];
        edgevertices[1] = vertices[2];
        edge.vertices(edgevertices);
        elements[1] = seg.push_back(edge, NULL);

        edgevertices[0] = vertices[1];
        edgevertices[1] = vertices[3];
        edge.vertices(edgevertices);
        elements[2] = seg.push_back(edge, NULL);

        edgevertices[0] = vertices[2];
        edgevertices[1] = vertices[3];
        edge.vertices(edgevertices);
        elements[3] = seg.push_back(edge, NULL);
      }
    };
  }
    
}

#endif

