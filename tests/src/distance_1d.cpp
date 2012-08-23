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

#include <cmath>

#include "viennagrid/forwards.h"
#include "viennagrid/element.hpp"
#include "viennagrid/point.hpp"
#include "viennagrid/domain.hpp"
#include "viennagrid/config/simplex.hpp"
#include "viennagrid/config/others.hpp"
#include "viennagrid/algorithm/distance.hpp"

void fuzzy_check(double a, double b)
{
  if (a != b)
  {
    if (   (std::abs(a - b) / std::max( std::abs(a), std::abs(b) ) > 1e-10) 
        && (std::abs(a - b) > 1e-10)
    )
    {
      std::cerr << "FAILED!" << std::endl;
      std::cerr << "Result mismatch: " << a << " vs. " << b << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  std::cout << "PASSED! (" << a << ", " << b << ")" << std::endl;
}


//
// Line 2d
//

void setup_domain(viennagrid::domain_t<viennagrid::config::line_1d> & d)
{
  typedef viennagrid::config::line_1d                             ConfigType;
  typedef ConfigType::cell_tag                                    CellTag;
  
  typedef viennagrid::result_of::point<ConfigType>::type          PointType;
  typedef viennagrid::result_of::ncell<ConfigType, 0>::type       VertexType;
  
  typedef viennagrid::result_of::ncell<ConfigType,
                                       CellTag::dim>::type        CellType;
  
  PointType p0(2.0);
  PointType p1(3.0);
  PointType p2(5.0);
  PointType p3(6.0);
  PointType p4(2.5);
  PointType p5(5.5);

  //upgrade to vertex:
  VertexType v0(p0, 0);
  VertexType v1(p1, 1);
  VertexType v2(p2, 2);
  VertexType v3(p3, 3);
  VertexType v4(p4, 4);
  VertexType v5(p5, 5);

  VertexType * vertices[6];
  
  std::cout << "Adding vertices to domain..." << std::endl;
  vertices[0] = d.push_back(v0);
  vertices[1] = d.push_back(v1);
  vertices[2] = d.push_back(v2);
  vertices[3] = d.push_back(v3);
  vertices[4] = d.push_back(v4);
  vertices[5] = d.push_back(v5);

  std::cout << "Adding cells to domain..." << std::endl;
  CellType line;
  
  line.vertices(vertices);
  d.push_back(line);

  vertices[0] = &(viennagrid::ncells<0>(d)[2]);
  vertices[1] = &(viennagrid::ncells<0>(d)[3]);
  line.vertices(vertices);
  d.push_back(line);
  
  vertices[0] = &(viennagrid::ncells<0>(d)[4]);
  vertices[1] = &(viennagrid::ncells<0>(d)[5]);
  line.vertices(vertices);
  d.push_back(line);
  
}

void test(viennagrid::config::line_1d)
{
  typedef viennagrid::config::line_1d                             ConfigType;
  typedef viennagrid::result_of::domain<ConfigType>::type               Domain;
  typedef ConfigType::cell_tag                                          CellTag;
  typedef viennagrid::segment_t<ConfigType>                             SegmentType;
  
  typedef viennagrid::result_of::point<ConfigType>::type                PointType;
  typedef viennagrid::result_of::ncell<ConfigType, 0>::type             VertexType;
  typedef viennagrid::result_of::ncell<ConfigType, 1>::type             EdgeType;
  typedef viennagrid::result_of::ncell<ConfigType, CellTag::dim>::type  CellType;

  Domain domain;
  
  setup_domain(domain);
  
  PointType A(0);
  PointType B(1.0);
  PointType C(1.5);
  PointType D(2.0);
  PointType E(2.5);
  PointType F(3.0);
  PointType G(3.5);
  PointType H(4.0);
  
  CellType line0 = viennagrid::ncells<CellTag::dim>(domain)[0];
  CellType line1 = viennagrid::ncells<CellTag::dim>(domain)[1];
  CellType line2 = viennagrid::ncells<CellTag::dim>(domain)[2];
  
  //
  // Distance checks for t0
  //
  
  std::cout << "Distance of point A to line0... ";
  fuzzy_check( viennagrid::distance(A, line0), 2.0 );
  
  std::cout << "Distance of point B to line0... ";
  fuzzy_check( viennagrid::distance(B, line0), 1.0 );

  std::cout << "Distance of point C to line0... ";
  fuzzy_check( viennagrid::distance(C, line0), 0.5 );
  
  std::cout << "Distance of point D to line0... ";
  fuzzy_check( viennagrid::distance(D, line0), 0 );
  
  std::cout << "Distance of point E to line0... ";
  fuzzy_check( viennagrid::distance(E, line0), 0 );
  
  std::cout << "Distance of point F to line0... ";
  fuzzy_check( viennagrid::distance(F, line0), 0 );
  
  std::cout << "Distance of point G to line0... ";
  fuzzy_check( viennagrid::distance(G, line0), 0.5 );
  
  std::cout << "Distance of point H to line0... ";
  fuzzy_check( viennagrid::distance(H, line0), 1.0 );

  std::cout << "Distance of point H to line0... ";
  fuzzy_check( viennagrid::distance(H, line0), 1.0 );
  
  std::cout << std::endl;
  
  //
  // Distance line to line
  //
  
  std::cout << "Distance of line0 to line0... ";
  fuzzy_check( viennagrid::distance(line0, line0), 0.0 );
  
  std::cout << "Distance of line0 to line1... ";
  fuzzy_check( viennagrid::distance(line0, line1), 2.0 );

  std::cout << "Distance of line0 to line2... ";
  fuzzy_check( viennagrid::distance(line0, line2), 0.0 );

  
  std::cout << "Distance of line1 to line0... ";
  fuzzy_check( viennagrid::distance(line1, line0), 2.0 );
  
  std::cout << "Distance of line1 to line1... ";
  fuzzy_check( viennagrid::distance(line1, line1), 0.0 );
  
  std::cout << "Distance of line1 to line2... ";
  fuzzy_check( viennagrid::distance(line1, line2), 0.0 );

  
  std::cout << "Distance of line1 to line0... ";
  fuzzy_check( viennagrid::distance(line2, line0), 0.0 );
  
  std::cout << "Distance of line1 to line1... ";
  fuzzy_check( viennagrid::distance(line2, line1), 0.0 );
  
  std::cout << "Distance of line1 to line2... ";
  fuzzy_check( viennagrid::distance(line2, line2), 0.0 );
  
  
}



int main()
{
  std::cout << "*****************" << std::endl;
  std::cout << "* Test started! *" << std::endl;
  std::cout << "*****************" << std::endl;

  std::cout << "==== Testing line mesh in 1D ====" << std::endl;
  test(viennagrid::config::line_1d());
  
  std::cout << "*******************************" << std::endl;
  std::cout << "* Test finished successfully! *" << std::endl;
  std::cout << "*******************************" << std::endl;
  
  return EXIT_SUCCESS;
}