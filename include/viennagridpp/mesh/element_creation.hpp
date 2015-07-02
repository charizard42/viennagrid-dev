#ifndef VIENNAGRID_ELEMENT_CREATION_HPP
#define VIENNAGRID_ELEMENT_CREATION_HPP

#include <vector>
#include "viennagridpp/forwards.hpp"
#include "viennagridpp/range.hpp"
#include "viennagridpp/mesh/region.hpp"

#include "viennagridpp/algorithm/detail/numeric.hpp"

namespace viennagrid
{
  inline element_t make_vertex(viennagrid_mesh mesh,
                               point_t const & point)
  {
    viennagrid_mesh_hierarchy mesh_hierarchy;
    viennagrid_mesh_mesh_hierarchy_get(mesh, &mesh_hierarchy);

    if ( viennagrid::geometric_dimension(mesh_hierarchy) <= 0 && !point.empty() )
      viennagrid_mesh_hierarchy_geometric_dimension_set( mesh_hierarchy, point.size() );

    viennagrid_int id;
    viennagrid_mesh_vertex_create( mesh, &point[0], &id );
    return element_t(mesh_hierarchy, 0, id);
  }


  inline element_t make_vertex(mesh_t const & mesh,
                               point_t const & point)
  {
    return make_vertex( mesh.internal(), point );
  }

  inline element_t make_vertex(mesh_t const & mesh)
  {
    return make_vertex( mesh.internal(), point_t() );
  }


  inline element_t make_vertex(mesh_region_t const & mesh_region, point_t const & point)
  {
    element_t vertex = make_vertex(mesh_region.internal_mesh(), point);
    add(mesh_region, vertex);
    return vertex;
  }

  inline element_t make_vertex(mesh_region_t const & mesh_region)
  {
    element_t vertex = make_vertex(mesh_region.internal_mesh(), point_t());
    add(mesh_region, vertex);
    return vertex;
  }





  template<typename SomethingT, typename NumericConfigT>
  typename result_of::element<SomethingT>::type make_unique_vertex(
        SomethingT something,
        typename result_of::point<SomethingT>::type const & point,
        NumericConfigT nc)
  {
    typedef typename result_of::const_vertex_range<SomethingT>::type VertexRangeType;
    typedef typename result_of::iterator<VertexRangeType>::type VertexRangeIterator;

    if (nc > 0)
    {
      VertexRangeType vertices(something);
      for (VertexRangeIterator vit = vertices.begin(); vit != vertices.end(); ++vit)
      {
        if ( detail::is_equal(nc, point, viennagrid::get_point(something, *vit)) )
          return *vit;
      }
    }

    return make_vertex(something, point);
  }









  template<typename ElementIteratorT>
  element_t make_element(viennagrid_mesh mesh,
                         element_tag et,
                         ElementIteratorT elements_begin,
                         ElementIteratorT elements_end)
  {
    viennagrid_mesh_hierarchy mesh_hierarchy;
    viennagrid_mesh_mesh_hierarchy_get(mesh, &mesh_hierarchy);

    std::vector<viennagrid_int> internal_vertices_indices;

    for (; elements_begin != elements_end; ++elements_begin)
    {
      internal_vertices_indices.push_back( (*elements_begin).id() );
    }

    viennagrid_int id;
    viennagrid_mesh_element_create(mesh,
                                   et.internal(),
                                   internal_vertices_indices.size(),
                                   &internal_vertices_indices[0],
                                   &id);

    return element_t(mesh_hierarchy, et.topologic_dimension(), id);
  }




  template<typename ElementIteratorT>
  element_t make_element(mesh_t const & mesh,
                         element_tag et,
                         ElementIteratorT elements_begin,
                         ElementIteratorT elements_end)
  {
    return make_element(mesh.internal(), et, elements_begin, elements_end);
  }

  template<typename ElementTagT, typename ElementIteratorT>
  element_t make_element(mesh_t const & mesh,
                         ElementIteratorT elements_begin,
                         ElementIteratorT elements_end)
  {
    return make_element(mesh.internal(), ElementTagT(), elements_begin, elements_end);
  }





  template<typename ElementIteratorT>
  element_t make_element(mesh_region_t const & mr,
                         element_tag et,
                         ElementIteratorT elements_begin,
                         ElementIteratorT elements_end)
  {
    element_t element = make_element(mr.internal_mesh(), et, elements_begin, elements_end);
    add(mr, element);
    return element;
  }

  template<typename ElementTagT, typename ElementIteratorT>
  element_t make_element(mesh_region_t const & mr,
                         ElementIteratorT elements_begin,
                         ElementIteratorT elements_end)
  {
    element_t element = make_element(mr.internal_mesh(), ElementTagT(), elements_begin, elements_end);
    add(mr, element);
    return element;
  }






  template<typename ElementIteratorT, typename IntersectionIteratorT>
  element_t make_refined_element(mesh_t const & mesh,
                                 element_t parent,
                                 element_tag et,
                                 ElementIteratorT elements_begin,
                                 ElementIteratorT elements_end,
                                 IntersectionIteratorT intersects_begin,
                                 IntersectionIteratorT intersects_end)
  {
    std::vector<viennagrid_int> internal_vertices_indices;

    for (; elements_begin != elements_end; ++elements_begin)
    {
      internal_vertices_indices.push_back( (*elements_begin).id() );
    }

    std::vector<viennagrid_int> vertex_indices;
    std::vector<viennagrid_dimension> intersects_dimensions;
    std::vector<viennagrid_int> intersects_indices;

    for (; intersects_begin != intersects_end; ++intersects_begin)
    {
      vertex_indices.push_back( (intersects_begin->first).id() );
      intersects_dimensions.push_back( viennagrid::topologic_dimension(intersects_begin->second) );
      intersects_indices.push_back( (intersects_begin->second).id() );
    }


    viennagrid_int id;
    viennagrid_mesh_refined_element_create(mesh.internal(),
                                           viennagrid::topologic_dimension(parent),
                                           parent.id(),
                                           et.internal(),
                                           internal_vertices_indices.size(),
                                           &internal_vertices_indices[0],
                                           vertex_indices.size(),
                                           &vertex_indices[0],
                                           &intersects_indices[0], &intersects_dimensions[0],
                                           &id);

    return element_t(mesh.mesh_hierarchy(), et.topologic_dimension(), id);
  }







  template<typename SomethingT>
  element_t make_line(SomethingT const & something, element_t v0, element_t v1)
  {
    element_t v[2];
    v[0] = v0;
    v[1] = v1;
    return make_element<line_tag>(something, v, v+2);
  }

  template<typename SomethingT>
  element_t make_triangle(SomethingT const & something, element_t v0, element_t v1, element_t v2)
  {
    element_t v[3];
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
    return make_element<triangle_tag>(something, v, v+3);
  }

  template<typename SomethingT>
  element_t make_quadrilateral(SomethingT const & something, element_t v0, element_t v1, element_t v2, element_t v3)
  {
    element_t v[4];
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
    v[3] = v3;
    return make_element<quadrilateral_tag>(something, v, v+4);
  }

  template<typename SomethingT>
  element_t make_tetrahedron(SomethingT const & something, element_t v0, element_t v1, element_t v2, element_t v3)
  {
    element_t v[4];
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
    v[3] = v3;
    return make_element<tetrahedron_tag>(something, v, v+4);
  }

  template<typename SomethingT>
  element_t make_hexahedron(SomethingT const & something, element_t v0, element_t v1, element_t v2, element_t v3,
                                                  element_t v4, element_t v5, element_t v6, element_t v7)
  {
    element_t v[8];
    v[0] = v0;
    v[1] = v1;
    v[2] = v2;
    v[3] = v3;
    v[4] = v4;
    v[5] = v5;
    v[6] = v6;
    v[7] = v7;
    return make_element<hexahedron_tag>(something, v, v+8);
  }

}

#endif
