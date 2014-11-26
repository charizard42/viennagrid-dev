#ifndef VIENNAGRID_MESH_HIERARCHY_HPP
#define VIENNAGRID_MESH_HIERARCHY_HPP

namespace viennagrid
{

  template<bool is_const>
  class base_mesh_hierarchy
  {
    template<bool other_is_const>
    friend class base_mesh_hierarchy;

    template<bool other_is_const>
    friend class base_mesh;

  public:

    typedef typename result_of::const_nonconst<mesh_t, is_const>::type mesh_type;
    typedef typename result_of::const_nonconst<mesh_t, true>::type const_mesh_type;

    typedef typename result_of::const_nonconst<region_t, is_const>::type region_type;
    typedef typename result_of::const_nonconst<region_t, true>::type const_region_type;

    typedef typename result_of::const_nonconst<element_t, is_const>::type element_type;
    typedef typename result_of::const_nonconst<element_t, true>::type const_element_type;

    typedef viennagrid_index region_id_type;
    typedef element_tag_t element_tag_type;


    base_mesh_hierarchy(viennagrid_mesh_hierarchy internal_mesh_hierarchy_in) : internal_mesh_hierarchy_(internal_mesh_hierarchy_in)
    {
      retain();
    }

    base_mesh_hierarchy() : internal_mesh_hierarchy_(0)
    {
      viennagrid_mesh_hierarchy_create(&internal_mesh_hierarchy_);
    }

    template<bool other_is_const>
    base_mesh_hierarchy(base_mesh_hierarchy<other_is_const> mh) : internal_mesh_hierarchy_(mh.internal_mesh_hierarchy_) {
      retain();
    }

    base_mesh_hierarchy(base_mesh_hierarchy<is_const> const & mh) : internal_mesh_hierarchy_(mh.internal_mesh_hierarchy_)
    {
      retain();
    }

    base_mesh_hierarchy<is_const> & operator=( base_mesh_hierarchy<is_const> const & mh )
    {
      release();
      internal_mesh_hierarchy_ = mh.internal_mesh_hierarchy_;
      retain();
      return *this;
    }

    ~base_mesh_hierarchy()
    {
      release();
    }

    mesh_type root();
    const_mesh_type root() const;

    viennagrid_mesh_hierarchy internal() const { return const_cast<viennagrid_mesh_hierarchy>(internal_mesh_hierarchy_); }

    std::size_t region_count() const
    {
      viennagrid_region * begin;
      viennagrid_region * end;
      viennagrid_regions_get(internal(), &begin, &end);
      return end-begin;
    }
    region_type get_make_region(region_id_type region_id);
    region_type make_region();
    const_region_type get_region(region_id_type region_id) const;

    region_type get_make_region(std::string const & name);
    const_region_type get_region(std::string const & name) const;

  private:

    void retain() const { viennagrid_mesh_hierarchy_retain(internal()); }
    void release() const { viennagrid_mesh_hierarchy_release(internal()); }

    viennagrid_mesh_hierarchy internal_mesh_hierarchy_;
  };


  template<bool lhs_is_const, bool rhs_is_const>
  bool operator==(base_mesh_hierarchy<lhs_is_const> const & lhs, base_mesh_hierarchy<rhs_is_const> const & rhs)
  { return lhs.internal() == rhs.internal(); }

  template<bool lhs_is_const, bool rhs_is_const>
  bool operator!=(base_mesh_hierarchy<lhs_is_const> const & lhs, base_mesh_hierarchy<rhs_is_const> const & rhs)
  { return !(lhs == rhs); }



  template<bool is_const>
  base_mesh_hierarchy<is_const> mesh_hierarchy( base_mesh_hierarchy<is_const> mesh_hierarchy )
  {
    return mesh_hierarchy;
  }



  template<bool is_const>
  viennagrid_int geometric_dimension( base_mesh_hierarchy<is_const> const & mesh_hierarchy )
  {
    viennagrid_int tmp;
    viennagrid_mesh_hierarchy_get_geometric_dimension(mesh_hierarchy.internal(), &tmp);
    return tmp;
  }

  template<typename SomethingT>
  viennagrid_int geometric_dimension( SomethingT something )
  {
    return geometric_dimension( mesh_hierarchy(something) );
  }




  template<bool is_const>
  viennagrid_int topologic_dimension( base_mesh_hierarchy<is_const> const & mesh_hierarchy )
  {
    viennagrid_int tmp;
    viennagrid_mesh_hierarchy_get_cell_dimension(mesh_hierarchy.internal(), &tmp);
    return tmp;
  }

  template<typename SomethingT>
  viennagrid_int topologic_dimension( SomethingT const & something )
  {
    return topologic_dimension( mesh_hierarchy(something) );
  }



  template<typename SomethingT>
  viennagrid_int cell_dimension( SomethingT const & something )
  {
    return topologic_dimension(mesh_hierarchy(something));
  }

  template<typename SomethingT>
  viennagrid_int facet_dimension( SomethingT const & something )
  {
    return cell_dimension(mesh_hierarchy(something)) - 1;
  }




  template<bool mesh_hierarchy_is_const, bool element_is_const>
  typename viennagrid::result_of::point< base_mesh_hierarchy<mesh_hierarchy_is_const> >::type get_point(
    base_mesh_hierarchy<mesh_hierarchy_is_const> mesh_hierarchy,
    base_element<element_is_const> const & vertex);

  void set_point(base_mesh_hierarchy<false> mesh_hierarchy,
                 base_element<false> const & vertex,
                 viennagrid::result_of::point< base_mesh_hierarchy<false> >::type const & point);


}


#endif
