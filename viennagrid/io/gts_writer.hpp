/* =======================================================================
   Copyright (c) 2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                             -----------------
                     ViennaMesh - The Vienna Mesh Library
                             -----------------

   authors:    Karl Rupp                          rupp@iue.tuwien.ac.at
               Peter Lagger                       peter.lagger@ieee.org

   license:    MIT (X11), see file LICENSE in the ViennaMesh base directory
======================================================================= */

#ifndef VIENNAMESH_IO_GTS_WRITER_GUARD
#define VIENNAMESH_IO_GTS_WRITER_GUARD

#include <fstream>
#include <iostream>
#include "viennamesh/forwards.h"
#include "viennamesh/io/helper.hpp"

#include "gtsio2/UtilityLibs/gtsio2/src/include/gtsio2.h"
#include <X11/Intrinsic.h>
#include "gts_list_getter.hpp"
#include <viennamesh/point.hpp>

using namespace std;
using namespace GTSIO::Data;
using namespace GTSIO::Reader;
using namespace GTSIO::Reader::CRV;
using namespace GTSIO;

namespace viennamesh
{
  template<long dim>
  struct gts_coords_getter;
  
  //********************************************************
  // struct gts_coords_getter
  // ------------------------
  // Gives the coords of a vertex in ViennaMesh
  //********************************************************
  // one dimension
  template<>
  struct gts_coords_getter<1> {
    static vector<double> getCoords(point_t<double,OneDimensionTag>& p) {
      vector<double> coords;
      coords.push_back(p.get_x());
      return coords;
    }
  };
  
  // two dimensions
  template<>
  struct gts_coords_getter<2> {
    static vector<double> getCoords(point_t<double,TwoDimensionsTag>& p) {
      vector<double> coords;
      coords.push_back(p.get_x());
      coords.push_back(p.get_y());
      return coords;
    }
  };
  
  // three dimensions
  template<>
  struct gts_coords_getter<3> {
    static vector<double> getCoords(point_t<double,ThreeDimensionsTag>& p) {
      vector<double> coords;
      coords.push_back(p.get_x());
      coords.push_back(p.get_y());
      coords.push_back(p.get_z());
      return coords;
    }
  };
  
  //**************************************************
  // struct gts_point_adder
  // ----------------------
  // Adds a vertex to a GTSObject:
  // 1D GTSLine
  // 2D GTSFace
  // 3D GTSPolyhedron
  //**************************************************
  template<long dim, typename obj>
  struct gts_point_adder;
  
  // one dimension
  template<typename obj>
  struct gts_point_adder<1, obj> {
    static void addPoints(obj* pO, vector<int> points) {
      pO->Init(points[0], points[1]);
    }
  };
  
  // two dimensions
  template<typename obj>
  struct gts_point_adder<2, obj> {
    static void addPoints(obj* pO, vector<int> points) {
      pO->Init(points);
    }
  };
  
  // three dimensions
  template<typename obj>
  struct gts_point_adder<3, obj> {
    static void addPoints(obj* pO, vector<int> points) {
      pO->Init(points);
    }
  };
  
  //***********************************
  // .gts-file writer
  //***********************************
  struct gts_writer
  {
    template <typename DomainType>
    int operator()(DomainType & domain, std::string const & filename)
    {
      typedef typename DomainType::Configuration                     DomainConfiguration;
    
      typedef typename DomainConfiguration::CoordType                 CoordType;
      typedef typename DomainConfiguration::DimensionTag              DimensionTag;
      typedef typename DomainConfiguration::CellTag                   CellTag;
      typedef typename DomainConfiguration::BoundaryReadTag           BoundaryReadTag;
    
      typedef typename DomainTypes<DomainConfiguration>::PointType    Point;
      typedef typename DomainTypes<DomainConfiguration>::VertexType   Vertex;
      typedef typename DomainTypes<DomainConfiguration>::CellType     Cell;
      typedef typename DomainTypes<DomainConfiguration>::SegmentType     Segment;
    
      typedef typename DomainTypes<DomainConfiguration>::VertexIterator      VertexIterator;
      typedef typename DomainTypes<DomainConfiguration>::CellIterator        CellIterator;

      typedef typename DomainTypes<DomainConfiguration>::VertexOnCellIterator      VertexOnCellIterator;
      
      typedef typename gts_list_getter<DimensionTag::dim>::list_type       GTSObjList;
      typedef typename gts_list_getter<DimensionTag::dim>::object_type     GTSObj;
      
      try {
        Segment & segment = *(domain.begin());
          
        GTSIO::DeviceFileWriter* pWriter = new GTSIO::DeviceFileWriter();
        GTSDevice*               pDevice = new GTSDevice();
        
        /* Set up the basic device properties */
        pDevice->setDimension(DimensionTag::dim);
        pDevice->setComment("This file is auto-generated from ViennaMesh.");
        pDevice->setName("Device");
        pDevice->setVersion("0.8");    

        cout << "Start writing the vertices! (GTSWriter)" << endl;
        cout << "Dimension: " << DimensionTag::dim << endl;
        
        //***************************************************************
        // Writing the vertices
        //***************************************************************
        GTSPointList* pPointList = pDevice->NewGTSPointList("Vertices");
        
        for (VertexIterator vit = segment.template begin<0>();
            vit != segment.template end<0>();
            ++vit)
        {
          Vertex & vertex = *vit;
          vector<double> pointCoords;
          
          cout << "Vertex Id: " << vertex.getID() << ", ";
          cout << vertex.getPoint() << endl;

          pointCoords = gts_coords_getter<DimensionTag::dim>::getCoords(vertex.getPoint());
          
          pPointList->add(pointCoords);
        }
          
        cout << "End writing the vertices!" << endl;
        cout << "Start writing the cells!" << endl;
          
        //***********************************************************
        // Writing the cells
        // ---------------------------------------------------------    
        // In GTS there are several concepts for a
        // cell in ViennaMesh depending on the dimension
        // ---------------------------------------------------------
        //                 1D           2D           3D
        // GTS            line         face      polyhedron
        // ViennaMesh     cell         cell         cell
        //***********************************************************   
        GTSObjList* pObjList = gts_list_getter<DimensionTag::dim>::newList(pDevice);
        
        long pointID;
        
        //segment.template size<CellTag::TopoLevel>();
        for (CellIterator cit = segment.template begin<CellTag::TopoLevel>();
            cit != segment.template end<CellTag::TopoLevel>();
            ++cit)
        {
          Cell & cell = *cit;
          GTSObj* pObj = new GTSObj();
          vector<int> points;
          
          cout << "Cell Id: " << cell.getID() << ":" << endl;

          for (VertexOnCellIterator vocit = cell.template begin<0>();
            vocit != cell.template end<0>();
            ++vocit)
          {	  
            pointID = vocit->getID();
            points.push_back(pointID);
            
            cout << "Point Id: " << pointID << " ";
          }

          gts_point_adder<DimensionTag::dim, GTSObj>::addPoints(pObj, points);
          
          pObjList->add(pObj);

          cout << "Address: " << pObj << endl;
          cout << endl;
        }
         
        // defines that the obj-list (lines, faces, polyhedrons) is build up of points (vertices)
        pObjList->setParentList(pPointList);
          
        cout << "End writing the cells!" << endl;     
          
        pWriter->WriteDeviceToFile("gts", filename.c_str(), pDevice);
        
        delete pWriter;
        delete pDevice;
      } catch(...) {
        cerr << "Error: Problem while writing the file!";
      }
    } //operator()
  }; // gts_writer
} //namespace

#endif


