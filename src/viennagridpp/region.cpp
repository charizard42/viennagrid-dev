#include "viennagrid/core/range.hpp"

namespace viennagrid
{

 void add(viennagrid_region region, element const & e)
  {
    viennagrid_region_element_add( region, e.id() );
  }


  template<bool element_is_const>
  bool is_in_region( viennagrid_region region, base_element<element_is_const> const & element )
  {
    viennagrid_region_id region_id;
    viennagrid_region_id_get(region, &region_id);

    viennagrid_region_id * it;
    viennagrid_region_id * end;

    viennagrid_element_regions_get(internal_mesh(element),
                                   element.id(),
                                   &it,
                                   &end);

    for (; it != end; ++it)
    {
      if (*it == region_id)
        return true;
    }

    return false;
  }

  template bool is_in_region( viennagrid_region region, base_element<false> const & element );
  template bool is_in_region( viennagrid_region region, base_element<true> const & element );




  template<bool element_is_const, bool region_is_const>
  bool is_boundary( base_region<region_is_const> const & region, base_element<element_is_const> const & element )
  {
    viennagrid_bool result;
    viennagrid_element_is_region_boundary(region.internal(),
                                          region.get_mesh().internal(),
                                          element.id(),
                                          &result);
    return result == VIENNAGRID_TRUE;
  }

  template bool is_boundary( base_region<false> const & region, base_element<false> const & element );
  template bool is_boundary( base_region<false> const & region, base_element<true> const & element );
  template bool is_boundary( base_region<true> const & region, base_element<false> const & element );
  template bool is_boundary( base_region<true> const & region, base_element<true> const & element );


}