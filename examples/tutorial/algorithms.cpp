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

#ifdef _MSC_VER      //Visual Studio complains about potentially dangerous things, which are perfectly legal in our context
  #pragma warning( disable : 4355 )     //use of this in member initializer list
  #pragma warning( disable : 4503 )     //truncated name decoration
#endif


#include "viennagrid/forwards.hpp"
#include "viennagrid/config/domain_config.hpp"
#include "viennagrid/io/netgen_reader.hpp"
#include "viennagrid/io/vtk_writer.hpp"

//Point-based algorithms:
#include "viennagrid/algorithm/cross_prod.hpp"
#include "viennagrid/algorithm/inner_prod.hpp"
#include "viennagrid/algorithm/norm.hpp"
#include "viennagrid/algorithm/spanned_volume.hpp"

//Cell-based algorithms:
#include "viennagrid/algorithm/centroid.hpp"
#include "viennagrid/algorithm/circumcenter.hpp"
#include "viennagrid/algorithm/surface.hpp"
#include "viennagrid/algorithm/volume.hpp"
#include "viennagrid/algorithm/closest_points.hpp"
#include "viennagrid/algorithm/distance.hpp"

//Domain-based algorithms:
#include "viennagrid/algorithm/boundary.hpp"
#include "viennagrid/algorithm/refine.hpp"
// #include "viennagrid/algorithm/voronoi.hpp"


#include <typeinfo>



class my_domain_config
{
  private:
    //typedef viennagrid::storage::pointer_handle_tag handle_tag;
    //typedef viennagrid::storage::iterator_handle_tag handle_tag;
    typedef viennagrid::storage::id_handle_tag          handle_tag;
    
  public:
    
    typedef viennagrid::point_t<double, viennagrid::cartesian_cs<3> > PointType;
    typedef viennagrid::config::result_of::full_domain_config< viennagrid::tetrahedron_tag, PointType, viennagrid::storage::pointer_handle_tag >::type type;
};


int main()
{
    
  typedef my_domain_config::PointType PointType;
  
  typedef viennagrid::domain_t< my_domain_config > Domain;  
  typedef viennagrid::result_of::domain_view<Domain>::type Segment;
  
  typedef viennagrid::tetrahedron_tag CellTag;
  
  typedef viennagrid::result_of::element<Domain, viennagrid::tetrahedron_tag>::type    CellType;
  typedef viennagrid::result_of::element<Domain, viennagrid::triangle_tag>::type       TriangleType;
  typedef viennagrid::result_of::element<Domain, viennagrid::line_tag>::type           EdgeType;
  typedef viennagrid::result_of::element<Domain, viennagrid::vertex_tag>::type         VertexType;
  typedef viennagrid::result_of::handle<Domain, viennagrid::vertex_tag>::type    VertexHandleType;
    
  typedef viennagrid::result_of::element_range<Domain, viennagrid::vertex_tag>::type       VertexRange;
  typedef viennagrid::result_of::element_range<Domain, viennagrid::tetrahedron_tag>::type       CellRange;

                      
  std::cout << "------------------------------------------------------------ " << std::endl;
  std::cout << "-- ViennaGrid tutorial: Algorithms on points and elements -- " << std::endl;
  std::cout << "------------------------------------------------------------ " << std::endl;
  std::cout << std::endl;
  
  Domain domain;
  std::vector<Segment> segments;
  
  //
  // Read domain from Netgen file
  //
  try
  {
    viennagrid::io::netgen_reader<CellType> reader;
    #ifdef _MSC_VER      //Visual Studio builds in a subfolder
    reader(domain, segments, "../../examples/data/cube48.mesh");
    #else
    reader(domain, segments, "../../examples/data/cube48.mesh");
    #endif
  }
  catch (std::exception & e)
  {
    std::cout << "Error reading Netgen mesh file: " << std::endl;
    std::cout << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  
  //
  // Part 1: Point-based algorithms:
  //

  // Extract the first four points of the domain:
//   VertexRange vertices = viennagrid::elements<viennagrid::vertex_tag>(domain);
//   
//   PointType const & p0 = viennagrid::point(domain, vertices[0]);
//   PointType const & p1 = viennagrid::point(domain, vertices[1]);
//   PointType const & p2 = viennagrid::point(domain, vertices[2]);
//   PointType const & p3 = viennagrid::point(domain, vertices[3]);
//   
//   std::cout << "Point p0: " << p0 << std::endl;
//   std::cout << "Point p1: " << p1 << std::endl;
//   std::cout << "Point p2: " << p2 << std::endl;
//   std::cout << "Point p3: " << p3 << std::endl;
//   
//   // Run a few algorithms:
//   std::cout << "Cross-product of p1 and p2: " << viennagrid::cross_prod(p1, p2) << std::endl;
//   std::cout << "Inner product of p1 and p2: " << viennagrid::inner_prod(p1, p2) << std::endl;
//   std::cout << "1-Norm of p2: "               << viennagrid::norm_1(p2) << std::endl;
//   std::cout << "2-Norm of p2: "               << viennagrid::norm_2(p2) << std::endl;
//   std::cout << "Inf-Norm of p2: "             << viennagrid::norm_inf(p2) << std::endl;
//   std::cout << "Length of line [p0, p1]: "    << viennagrid::spanned_volume(p0, p1) << std::endl;
//   std::cout << "Area of triangle [p0, p1, p2]: " << viennagrid::spanned_volume(p0, p1, p2) << std::endl;
//   std::cout << "Volume of tetrahedron [p0, p1, p2, p3]: " << viennagrid::spanned_volume(p0, p1, p2, p3) << std::endl;
//   
//   std::cout << std::endl << "--------------------------------" << std::endl << std::endl;
//   
//   
//   //
//   // Part 2: Cell-based algorithms:
//   //
//   
//   // Extract first cell from domain:
//   CellType const & cell = viennagrid::elements<CellTag>(domain)[0];
//   
//   std::cout << "cell: " << std::endl << cell << std::endl;
//   
//   // Run algorithms:
//   std::cout << "Centroid of cell: "     << viennagrid::centroid(cell) << std::endl;
//   std::cout << "Circumcenter of cell: " << viennagrid::circumcenter(cell) << std::endl;
//   std::cout << "Surface of cell: "      << viennagrid::surface(cell) << std::endl;
//   std::cout << "Volume of cell: "       << viennagrid::volume(cell) << std::endl;
//   std::cout << std::endl;
//   
//   std::cout << "Volume of domain: "       << viennagrid::volume(domain) << std::endl;
//   std::cout << "Surface of domain: "       << viennagrid::surface(domain) << std::endl;
  
//   CellType const & cell2 = viennagrid::elements<CellTag>(domain)[1];
//   std::cout << "Distance between cells: " << viennagrid::boundary_distance(cell, cell2) << std::endl;
  
  
  //
  // Part 3: Domain-based algorithms (except interfaces. Refer to the multi-segment tutorial multi_segment.cpp)
  //
  
  //
  // Write Voronoi info to default ViennaData keys:
//   viennagrid::apply_voronoi<CellType>(domain);
  
  // Write Voronoi info again, this time using custom keys
//   std::string interface_area_key = "voronoi_interface";
//   std::string box_volume_key = "voronoi_volume";
//   viennagrid::apply_voronoi<CellType>(domain, interface_area_key, box_volume_key);
//   
//   std::cout << "Voronoi box volume at first vertex: "
//             << viennadata::access<std::string, double>(box_volume_key)(vertices[0])
//             << std::endl;
  
            

  
  std::deque<bool> cell_refinement_flag;
  std::deque<bool> edge_refinement_flag;
  std::deque<VertexHandleType> vertex_refinement_vertex_handle;
  std::deque<VertexHandleType> edge_refinement_vertex_handle;
  
  cell_refinement_flag.resize( 1000 );
  edge_refinement_flag.resize( 1000 );
  vertex_refinement_vertex_handle.resize( 1000 );
  edge_refinement_vertex_handle.resize( 1000 );
  
  std::fill( cell_refinement_flag.begin(), cell_refinement_flag.end(), false );
  std::fill( edge_refinement_flag.begin(), edge_refinement_flag.end(), false );
  
//   cell_refinement_flag[0] = true;
  edge_refinement_flag[0] = true;
  
  //          
  // Refine domain uniformly:
  Domain uniformly_refined_domain;
  viennagrid::refine<viennagrid::tetrahedron_tag>(domain, uniformly_refined_domain,
                                                  viennagrid::accessor::dense_container_accessor<CellType>(cell_refinement_flag),
                                                  viennagrid::accessor::dense_container_accessor<EdgeType>(edge_refinement_flag),
                                                  viennagrid::accessor::dense_container_accessor<VertexType>(vertex_refinement_vertex_handle),
                                                  viennagrid::accessor::dense_container_accessor<EdgeType>(edge_refinement_vertex_handle));
  
  
//   std::deque<PointType> tmp_points;  
//   std::cout << viennagrid::centroid( viennagrid::accessor::dense_container_accessor<VertexType>(tmp_points), cell ) << std::endl;
  
  //Domain uniformly_refined_domain = viennagrid::refine(domain, viennagrid::uniform_refinement_tag()); //equivalent to previous line
    
  //
  // Adaptive refinement: Tag first three cells in domain for refinement
//   CellRange cells = viennagrid::elements<CellTag>(domain);
//   viennadata::access<viennagrid::refinement_key, bool>()(cells[0]) = true;
//   viennadata::access<viennagrid::refinement_key, bool>()(cells[1]) = true;
//   viennadata::access<viennagrid::refinement_key, bool>()(cells[2]) = true;
//   Domain adaptively_refined_domain = viennagrid::refine<CellTag>(domain, viennagrid::local_refinement_tag());
  //Domain adaptively_refined_domain = viennagrid::refine_adaptively(domain);  //equivalent to previous line
  
  
  viennagrid::io::vtk_writer<Domain, CellType> writer;
  writer(domain, "domain.vtu");
  writer(uniformly_refined_domain, "uniform_refinement.vtu");
//   writer(adaptively_refined_domain, "local_refinement");
  
  
  //
  // Get boundary information of first vertex with respect to the full domain:
            
//             for (VertexRange::iterator it = vertices.begin(); it != vertices.end(); ++it)
//   std::cout << *it << " " << viennagrid::point(domain, *it) << " "
//             << viennagrid::is_boundary(domain, *it)    //second argument is the enclosing complex (either a domain or a segment)
//             << std::endl << std::endl;
  
  
  
  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << " \\o/    Tutorial finished successfully!    \\o/ " << std::endl;
  std::cout << "-----------------------------------------------" << std::endl;

  return EXIT_SUCCESS;
}
