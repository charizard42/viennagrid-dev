#ifndef VIENNAGRID_SEGMENT_HPP
#define VIENNAGRID_SEGMENT_HPP

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


#include <map>
#include <set>
#include <deque>

#include "viennagrid/forwards.hpp"
#include "viennagrid/domain.hpp"   //segment does not make any sense without domain
#include "viennagrid/detail/segment_layers.hpp"
#include "viennagrid/detail/segment_iterators.hpp"

/** @file segment.hpp
    @brief Provides the segment type to be used for partitioning the domain
*/

namespace viennagrid
{
  
  
//   //Segment type: set up using recursive inheritance, similar to domain_t and element_t
//   template <typename Conf>
//   class segment_t : public detail::segment_layers_top<Conf, Conf::cell_tag::dim>
//   {
//     typedef detail::segment_layers_top<Conf, Conf::cell_tag::dim>     base_type;
//     
//     public:
//       /** @brief Provide access to the configuration class */
//       typedef Conf                config_type;
// 
//       /** @brief Destructor. Cleans up any data associated with the segment if it is destroyed */
//       ~segment_t() { viennadata::erase<viennadata::all, viennadata::all>()(*this); }
//       
//       using base_type::push_back;
// 
//   };

  
  
  
}
#endif
