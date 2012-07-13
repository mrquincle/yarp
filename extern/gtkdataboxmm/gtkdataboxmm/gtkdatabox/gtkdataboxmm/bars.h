// -*- c++ -*-
// Generated by gtkmmproc -- DO NOT MODIFY!
#ifndef _GTKDATABOXMM_BARS_H
#define _GTKDATABOXMM_BARS_H

#include <gtkdataboxmmconfig.h>


#include <glibmm/ustring.h>
#include <sigc++/sigc++.h>

/* Copyright (c) 2012  iCub Facility, Istituto Italiano di Tecnologia
 *   @author Daniele E. Domenichelli <daniele.domenichelli@iit.it>
 *
 * This file is part of gtkdataboxmm.
 *
 * gtkdataboxmm is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 2.1 of the License,
 * or (at your option) any later version.
 *
 * gtkdataboxmm is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <gtkdataboxmm/xyc_graph.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
typedef struct _GtkDataboxBars GtkDataboxBars;
typedef struct _GtkDataboxBarsClass GtkDataboxBarsClass;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */


namespace GDatabox
{ class Bars_Class; } // namespace GDatabox
namespace Gdk { class Color; }

namespace GDatabox
{


class Bars : public XYCGraph
{
    
#ifndef DOXYGEN_SHOULD_SKIP_THIS

public:
  typedef Bars CppObjectType;
  typedef Bars_Class CppClassType;
  typedef GtkDataboxBars BaseObjectType;
  typedef GtkDataboxBarsClass BaseClassType;

private:  friend class Bars_Class;
  static CppClassType bars_class_;

private:
  // noncopyable
  Bars(const Bars&);
  Bars& operator=(const Bars&);

protected:
  explicit Bars(const Glib::ConstructParams& construct_params);
  explicit Bars(GtkDataboxBars* castitem);

#endif /* DOXYGEN_SHOULD_SKIP_THIS */

public:
  virtual ~Bars();

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  static GType get_type()      G_GNUC_CONST;


  static GType get_base_type() G_GNUC_CONST;
#endif

  ///Provides access to the underlying C GObject.
  GtkDataboxBars*       gobj()       { return reinterpret_cast<GtkDataboxBars*>(gobject_); }

  ///Provides access to the underlying C GObject.
  const GtkDataboxBars* gobj() const { return reinterpret_cast<GtkDataboxBars*>(gobject_); }

  ///Provides access to the underlying C instance. The caller is responsible for unrefing it. Use when directly setting fields in structs.
  GtkDataboxBars* gobj_copy();

private:


protected:
    Bars(unsigned int length, float *X, float *Y, const Gdk::Color &color, unsigned int size = 1);
    

public:
    static Glib::RefPtr<Bars> create(unsigned int length, float * X, float * Y, const Gdk::Color &color, unsigned int size = 1);
    // FIXME implemented manually because gmmproc doesn't like unsigned int
    //_WRAP_CREATE(unsigned int length, float *X, float *Y, const Gdk::Color &color, unsigned int size)


public:

public:
  //C++ methods used to invoke GTK+ virtual functions:

protected:
  //GTK+ Virtual Functions (override these to change behaviour):

  //Default Signal Handlers::


};

} // namespace GDatabox


namespace Glib
{
  /** A Glib::wrap() method for this object.
   * 
   * @param object The C instance.
   * @param take_copy False if the result should take ownership of the C instance. True if it should take a new copy or ref.
   * @result A C++ instance that wraps this C instance.
   *
   * @relates GDatabox::Bars
   */
  Glib::RefPtr<GDatabox::Bars> wrap(GtkDataboxBars* object, bool take_copy = false);
}


#endif /* _GTKDATABOXMM_BARS_H */

