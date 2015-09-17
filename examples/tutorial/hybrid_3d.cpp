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

#include "viennagrid/viennagrid.hpp"
#include "viennagrid/io/vtk_writer.hpp"

//
//    Let us construct the following input mesh:
//
//    5---------4---------3
//    | \       |         |
//    |   \     |         |    y
//    |     \   |         |     ^
//    |       \ |         |     |
//    0---------1---------2     *--> x
//
//    Segment 1 | Segment 2
//


int main()
{
  //
  // Define the necessary types:
  //

  typedef viennagrid::mesh                                        MeshType;

  typedef viennagrid::result_of::element<MeshType>::type          VertexType;

  typedef viennagrid::result_of::element_range<MeshType>::type    ElementRange;
  typedef viennagrid::result_of::iterator<ElementRange>::type     ElementIterator;

  std::cout << "-------------------------------------------------------------- " << std::endl;
  std::cout << "-- ViennaGrid tutorial: Setup of a mesh with two segments -- " << std::endl;
  std::cout << "-------------------------------------------------------------- " << std::endl;
  std::cout << std::endl;

  //
  // Step 1: Instantiate the mesh and the segmentation and create 2 segments:
  //
  MeshType mesh;

//   RegionType region0 = mesh.make_region();
//   RegionType region1 = mesh.make_region();

  //
  // Step 2: Add vertices to the mesh.
  //         Note that vertices with IDs are enumerated in the order they are pushed to the mesh.
  //
  VertexType v0 = viennagrid::make_vertex(mesh, viennagrid::make_point(0, 0, 0) );
  VertexType v1 = viennagrid::make_vertex(mesh, viennagrid::make_point(2, 0, 0) );
  VertexType v2 = viennagrid::make_vertex(mesh, viennagrid::make_point(0, 1, 0) );
  VertexType v3 = viennagrid::make_vertex(mesh, viennagrid::make_point(2, 1, 0) );
  VertexType v4 = viennagrid::make_vertex(mesh, viennagrid::make_point(3.0, 0.5, 0.5) );
  VertexType v5 = viennagrid::make_vertex(mesh, viennagrid::make_point(0, 0, 1) );
  VertexType v6 = viennagrid::make_vertex(mesh, viennagrid::make_point(2, 0, 1) );
  VertexType v7 = viennagrid::make_vertex(mesh, viennagrid::make_point(0, 1, 1) );
  VertexType v8 = viennagrid::make_vertex(mesh, viennagrid::make_point(2, 1, 1) );
  VertexType v9 = viennagrid::make_vertex(mesh, viennagrid::make_point(0, 0.5, 1.5) );
  VertexType v10 = viennagrid::make_vertex(mesh, viennagrid::make_point(2, 0.5, 1.5) );
  VertexType v11 = viennagrid::make_vertex(mesh, viennagrid::make_point(-1, 0.5, 1.2) );

  //
  // Step 3: Fill the two segments with cells.
  //         To do so, each cell must be linked with the defining vertex handles from the mesh
  //

  // First triangle, use vertex handles
  viennagrid::make_hexahedron(mesh, v0, v1, v2, v3, v5, v6, v7, v8);
  viennagrid::make_pyramid(mesh, v1, v3, v6, v8, v4);
  viennagrid::make_wedge(mesh, v6, v8, v10, v5, v7, v9);
  viennagrid::make_tetrahedron(mesh, v5, v7, v9, v11);

  ElementRange cells(mesh, 3);
  std::cout << "Cells in mesh (count = " << cells.size() << "):" << std::endl;
  for (ElementIterator cit0 = cells.begin();
                      cit0 != cells.end();
                    ++cit0)
  {
    std::cout << *cit0 << std::endl;
  }
  std::cout << std::endl;


  viennagrid::io::vtk_writer<MeshType> writer;
  writer(mesh, "hybrid_3d");


  std::cout << "-----------------------------------------------" << std::endl;
  std::cout << " \\o/    Tutorial finished successfully!    \\o/ " << std::endl;
  std::cout << "-----------------------------------------------" << std::endl;

  return EXIT_SUCCESS;
}
