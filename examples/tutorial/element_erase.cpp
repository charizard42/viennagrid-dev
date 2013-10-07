#include <iostream>
#include <typeinfo>


#include "viennagrid/config/default_configs.hpp"
#include "viennagrid/mesh/element_creation.hpp"
#include "viennagrid/mesh/element_deletion.hpp"
#include "viennagrid/mesh/coboundary_iteration.hpp"
#include "viennagrid/io/vtk_writer.hpp"



int main()
{
    typedef viennagrid::triangular_2d_mesh MeshType;

    typedef viennagrid::result_of::point<MeshType>::type point_type;

    typedef viennagrid::result_of::vertex_handle<MeshType>::type VertexHandleType;

    MeshType mesh;

    VertexHandleType v00 = viennagrid::make_vertex(mesh, point_type(0.0, 0.0));
    VertexHandleType v10 = viennagrid::make_vertex(mesh, point_type(1.0, 0.0));
    VertexHandleType v20 = viennagrid::make_vertex(mesh, point_type(2.0, 0.0));
    VertexHandleType v01 = viennagrid::make_vertex(mesh, point_type(0.0, 1.0));
    VertexHandleType v11 = viennagrid::make_vertex(mesh, point_type(1.0, 1.0));
    VertexHandleType v21 = viennagrid::make_vertex(mesh, point_type(2.0, 1.0));
    VertexHandleType v02 = viennagrid::make_vertex(mesh, point_type(0.0, 2.0));
    VertexHandleType v12 = viennagrid::make_vertex(mesh, point_type(1.0, 2.0));
    VertexHandleType v22 = viennagrid::make_vertex(mesh, point_type(2.0, 2.0));


    viennagrid::make_triangle(mesh, v00, v01, v11);
    viennagrid::make_triangle(mesh, v00, v10, v11);
    viennagrid::make_triangle(mesh, v10, v11, v20);
    viennagrid::make_triangle(mesh, v11, v20, v21);
    viennagrid::make_triangle(mesh, v11, v21, v22);
    viennagrid::make_triangle(mesh, v12, v11, v22);
    viennagrid::make_triangle(mesh, v02, v11, v12);
    viennagrid::make_triangle(mesh, v01, v11, v02);


    std::cout << viennagrid::triangles(mesh).size() << std::endl;


    typedef viennagrid::result_of::mesh_view<MeshType>::type mesh_view_type;
    mesh_view_type elements_to_erase = viennagrid::make_view(mesh);
    viennagrid::mark_erase_elements( mesh, elements_to_erase, v22 );
    viennagrid::mark_erase_elements( mesh, elements_to_erase, v21 );

    viennagrid::erase_elements(mesh, elements_to_erase);

    std::cout << viennagrid::triangles(mesh).size() << std::endl;

    {
        viennagrid::io::vtk_writer<viennagrid::triangular_2d_mesh> vtk_writer;
        vtk_writer(mesh, "triangle_mesh");
    }

    return 0;
}
