/* =======================================================================
   Copyright (c) 2011-2014, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                     ViennaGrid - The Vienna Grid Library
                            -----------------

   License:      MIT (X11), see file LICENSE in the base directory
======================================================================= */

#ifdef _MSC_VER
  #pragma warning( disable : 4503 )     //truncated name decoration
#endif

#include <iostream>
#include <typeinfo>

#include "viennagridpp/core.hpp"


int main()
{
  typedef viennagrid::mesh                                          MeshType;

  typedef viennagrid::result_of::element< MeshType >::type          VertexType;
  typedef viennagrid::result_of::element< MeshType >::type          TriangleType;

  // defining the region type and region id type
  typedef   viennagrid::result_of::region<MeshType>::type RegionType;


  MeshType mesh;

  // create some vertices
  VertexType v0 = viennagrid::make_vertex( mesh );
  VertexType v1 = viennagrid::make_vertex( mesh );
  VertexType v2 = viennagrid::make_vertex( mesh );
  VertexType v3 = viennagrid::make_vertex( mesh );
  VertexType v4 = viennagrid::make_vertex( mesh );
  VertexType v5 = viennagrid::make_vertex( mesh );

  // create some triangles
  TriangleType tri0 = viennagrid::make_triangle( mesh, v0, v1, v2 );
  TriangleType tri1 = viennagrid::make_triangle( mesh, v1, v2, v3 );
  TriangleType tri2 = viennagrid::make_triangle( mesh, v3, v4, v5 );
  /*TriangleHandleType th3 =*/ viennagrid::make_triangle( mesh, v0, v4, v5 );



  // create a region, region id will be 0 because of first region
  RegionType region0 = mesh.create_region();

  // create a region, region id will be 4 because explicit region_id
  RegionType region1 = mesh.get_or_create_region(4);

  // create a region, region id will be 5 because highest region id was 4
  RegionType region2 = mesh.create_region();

  // create some other regions (ID would be 6, 7, 8)
  RegionType region3 = mesh.create_region();
  RegionType region4 = mesh.create_region();
  RegionType region5 = mesh.create_region();


  // print the regions
  std::cout << "Regions in Mesh" << std::endl;
  typedef viennagrid::result_of::region_range<MeshType>::type RegionRangeType;
  typedef viennagrid::result_of::iterator<RegionRangeType>::type RegionRangeIterator;

  RegionRangeType regions(mesh);
  for (RegionRangeIterator it = regions.begin(); it != regions.end(); ++it)
    std::cout << (*it).id() << std::endl;
  std::cout << std::endl;


  // add triangle tr0 to region0
  std::cout << "Adding element tri0 to region0" << std::endl;
  viennagrid::add( region0, tri0 );

  std::cout << "Adding element tri0 to region1" << std::endl;
  viennagrid::add( region1, tri0 );

  std::cout << "Triangle 0 in Region 0: " << viennagrid::is_in_region( region0, tri0 ) << std::endl;
  std::cout << "Vertex 0 in Region 0: " << viennagrid::is_in_region( region0, v0 ) << std::endl;
  std::cout << "Number of cells in regionation: " << viennagrid::cells(mesh).size() << std::endl;

  std::cout << "Erasing element tri0 from region0" << std::endl;
  // erase triangle tr0 from region0
//   viennagrid::erase( region0, tri0 );
//   std::cout << "Triangle 0 in Region 0: " <<  viennagrid::is_in_region( region0, tri0 ) << std::endl;
//   std::cout << "Vertex 0 in Region 0: " << viennagrid::is_in_region( region0, v0 ) << std::endl;
//   std::cout << "Number of cells in regionation: " << viennagrid::cells(mesh).size() << std::endl;

  std::cout << "Erasing element tri0 from region1" << std::endl;
  // erase triangle tr0 from region0
//   viennagrid::erase( region1, tri0 );
//   std::cout << "Triangle 0 in Region 0: " <<  viennagrid::is_in_region( region0, tri0 ) << std::endl;
//   std::cout << "Vertex 0 in Region 0: " << viennagrid::is_in_region( region0, v0 ) << std::endl;
//   std::cout << "Number of cells in regionation: " << viennagrid::cells(mesh).size() << std::endl;


  // add triangle tr0 to region0, tri1 to region1, tri2 to region2
  viennagrid::add( region0, tri0 );
  viennagrid::add( region1, tri1 );
  viennagrid::add( region2, tri2 );


  viennagrid::add( region3, tri0 );
  viennagrid::add( region4, tri0 );
  viennagrid::add( region5, tri0 );


  // print all cells of the mesh (should be four)
  typedef viennagrid::result_of::cell_range<MeshType>::type MeshCellRange;
  typedef viennagrid::result_of::iterator<MeshCellRange>::type MeshCellIterator;

  std::cout << std::endl;
  std::cout << "All cells in the mesh:" << std::endl;
  MeshCellRange mesh_cells( mesh );
  for (MeshCellIterator cit = mesh_cells.begin(); cit != mesh_cells.end(); ++cit)
    std::cout << *cit << std::endl;
  std::cout << std::endl;

  // print all cells of the mesh (should be three)
  typedef viennagrid::result_of::cell_range<MeshType>::type RegionCellRange;
  typedef viennagrid::result_of::iterator<RegionCellRange>::type RegionCellIterator;

  std::cout << std::endl;
  std::cout << "All cells in the regionation:" << std::endl;
  RegionCellRange region_cells(mesh);
  for (RegionCellIterator cit = region_cells.begin(); cit != region_cells.end(); ++cit)
    std::cout << *cit << std::endl;
  std::cout << std::endl;




  typedef viennagrid::result_of::region_range<MeshType, TriangleType>::type ElementRegionRange;
  typedef viennagrid::result_of::iterator<ElementRegionRange>::type ElementRegionIterator;

  ElementRegionRange element_regions(mesh, tri0);
  std::cout << "Regions for tr0:" << std::endl;
  for (ElementRegionIterator rit = element_regions.begin(); rit != element_regions.end(); ++rit)
    std::cout << (*rit).id() << std::endl;


//   // setting and querying additional region information for tri0
//   std::cout << "Triangle 0 in Region 0: " <<  *viennagrid::region_element_info( region0, tri0 ) << std::endl;
//   *viennagrid::region_element_info( region0, tri0 ) = true;
//   std::cout << "Triangle 0 in Region 0: " <<  *viennagrid::region_element_info( region0, tri0 ) << std::endl;
//   std::cout << std::endl;

  // printing all triangles from all regions
  typedef viennagrid::result_of::cell_range<RegionType>::type RangeType;

  std::cout << "Triangles of Region 0" << std::endl;
  RangeType range( region0 );
  for (RangeType::iterator it = range.begin(); it != range.end(); ++it)
    std::cout << *it << std::endl;

  std::cout << "Triangles of Region 1" << std::endl;
  range = RangeType( region1 );
  for (RangeType::iterator it = range.begin(); it != range.end(); ++it)
    std::cout << *it << std::endl;

  std::cout << "Triangles of Region 2" << std::endl;
  range = RangeType( region2 );
  for (RangeType::iterator it = range.begin(); it != range.end(); ++it)
    std::cout << *it << std::endl;

  std::cout << std::endl;

  // Adding additional triangles to region2 and printing them
  viennagrid::make_triangle( region2, v1, v4, v5 );
  viennagrid::make_triangle( region2, v2, v4, v5 );

  std::cout << "Triangles of Region 2, added 2 additional" << std::endl;
  range = RangeType( region2 );
  for (RangeType::iterator it = range.begin(); it != range.end(); ++it)
    std::cout << *it << std::endl;
  std::cout << std::endl;

  // Printing vertices from region2, each vertex should only be printed once
  typedef viennagrid::result_of::vertex_range<RegionType>::type VertexRange;
  std::cout << "Vertices of Region 2" << std::endl;
  VertexRange vertices( region2 );
  for (VertexRange::iterator it = vertices.begin(); it != vertices.end(); ++it)
    std::cout << *it << std::endl;

  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << " \\o/    Tutorial finished successfully!    \\o/ " << std::endl;
  std::cout << "-----------------------------------------------" << std::endl;

  return EXIT_SUCCESS;
}