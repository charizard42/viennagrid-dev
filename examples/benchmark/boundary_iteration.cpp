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

#include "viennagridpp/core.hpp"
#include "viennagridpp/algorithm/volume.hpp"
#include "viennagridpp/io/vtk_writer.hpp"

#include "common.hpp"
#include "bench.hpp"
#include "iteration.hpp"


int main()
{
  int bench_count = 10;
  int cell_count = 1000000;


  std::cout << "C pure triangles" << std::endl;
  bench_iteration(cell_count, bench_count, make_aabb_triangles_CPP, boost::bind(boundary_iteration_C_pure, _1, 0));
  std::cout << std::endl;

  std::cout << "C triangles" << std::endl;
  bench_iteration(cell_count, bench_count, make_aabb_triangles_CPP, boost::bind(boundary_iteration_C, _1, 0));
  std::cout << std::endl;

  std::cout << "CPP triangles" << std::endl;
  bench_iteration(cell_count, bench_count, make_aabb_triangles_CPP, boost::bind(boundary_iteration_CPP, _1, 0));
  std::cout << std::endl;


  std::cout << "C pure tetrahedrons" << std::endl;
  bench_iteration(cell_count, bench_count, make_aabb_tetrahedrons_CPP, boost::bind(boundary_iteration_C_pure, _1, 0));
  std::cout << std::endl;

  std::cout << "C tetrahedrons" << std::endl;
  bench_iteration(cell_count, bench_count, make_aabb_tetrahedrons_CPP, boost::bind(boundary_iteration_C, _1, 0));
  std::cout << std::endl;

  std::cout << "CPP tetrahedrons" << std::endl;
  bench_iteration(cell_count, bench_count, make_aabb_tetrahedrons_CPP, boost::bind(boundary_iteration_CPP, _1, 0));
  std::cout << std::endl;


  return EXIT_SUCCESS;
}