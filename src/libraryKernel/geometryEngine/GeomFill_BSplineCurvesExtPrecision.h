#ifndef _GeomFill_BSplineCurvesExtPrecision_HeaderFile
#define _GeomFill_BSplineCurvesExtPrecision_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>
#include <Precision.hxx>

#include <Handle_Geom_BSplineSurface.hxx>
#include <Handle_Geom_BSplineCurve.hxx>
#include <GeomFill_FillingStyle.hxx>
class Geom_BSplineSurface;
class Standard_ConstructionError;
class Geom_BSplineCurve;


class GeomFill_BSplineCurvesExtPrecision 
{
public:

  DEFINE_STANDARD_ALLOC

  
  //! Constructs a default BSpline surface framework.
  Standard_EXPORT GeomFill_BSplineCurvesExtPrecision();
  
  Standard_EXPORT GeomFill_BSplineCurvesExtPrecision(const Handle(Geom_BSplineCurve)& C1, const Handle(Geom_BSplineCurve)& C2, const Handle(Geom_BSplineCurve)& C3, const Handle(Geom_BSplineCurve)& C4, const GeomFill_FillingStyle Type);
  
  Standard_EXPORT GeomFill_BSplineCurvesExtPrecision(const Handle(Geom_BSplineCurve)& C1, const Handle(Geom_BSplineCurve)& C2, const Handle(Geom_BSplineCurve)& C3, const GeomFill_FillingStyle Type);
  
  //! Constructs a framework for building a BSpline surface from either
  //! -   the four contiguous BSpline curves, C1, C2, C3 and C4, or
  //! -   the three contiguous BSpline curves, C1, C2 and C3, or
  //! -   the two contiguous BSpline curves, C1 and C2.
  //! The type of filling style Type to be used is one of:
  //! -   GeomFill_Stretch - the style with the flattest patch
  //! -   GeomFill_Coons - a rounded style of patch with
  //! less depth than that of Curved
  //! -   GeomFill_Curved - the style with the most rounded
  //! patch.Constructs a framework for building a BSpline
  //! surface common to the two BSpline curves, C1 and C2.
  //! Exceptions
  //! Standard_ConstructionError if the curves are not contiguous.
  Standard_EXPORT GeomFill_BSplineCurvesExtPrecision(const Handle(Geom_BSplineCurve)& C1, const Handle(Geom_BSplineCurve)& C2, const GeomFill_FillingStyle Type);
  
  //! if the curves cannot be joined
  Standard_EXPORT   void Init (const Handle(Geom_BSplineCurve)& C1, const Handle(Geom_BSplineCurve)& C2, const Handle(Geom_BSplineCurve)& C3, const Handle(Geom_BSplineCurve)& C4, const GeomFill_FillingStyle Type, Standard_Real Tol = Precision::Confusion()) ;
  
  //! if the curves cannot be joined
  Standard_EXPORT   void Init (const Handle(Geom_BSplineCurve)& C1, const Handle(Geom_BSplineCurve)& C2, const Handle(Geom_BSplineCurve)& C3, const GeomFill_FillingStyle Type) ;
  
  //! Initializes or reinitializes this algorithm with two, three,
  //! or four curves - C1, C2, C3, and C4 - and Type, one
  //! of the following filling styles:
  //! -   GeomFill_Stretch - the style with the flattest patch
  //! -   GeomFill_Coons - a rounded style of patch with
  //! less depth than that of Curved
  //! -   GeomFill_Curved - the style with the most rounded patch.
  //! Exceptions
  //! Standard_ConstructionError if the curves are not contiguous.
  Standard_EXPORT   void Init (const Handle(Geom_BSplineCurve)& C1, const Handle(Geom_BSplineCurve)& C2, const GeomFill_FillingStyle Type) ;
  
  //! Returns the BSpline surface Surface resulting from
  //! the computation performed by this algorithm.
//     const  Handle(Geom_BSplineSurface)& Surface()  const;
const Handle(Geom_BSplineSurface)&  Surface()
     const 
{
  return mySurface;
}



protected:





private:



  Handle(Geom_BSplineSurface) mySurface;


};


//#include <GeomFill_BSplineCurvesExtPrecision.lxx>





#endif // _GeomFill_BSplineCurvesExtPrecision_HeaderFile
