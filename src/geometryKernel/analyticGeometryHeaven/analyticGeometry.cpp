/*---------------------------------------------------------------------------*\
  dtOO < design tool Object-Oriented >

    Copyright (C) 2024 A. Tismer.
-------------------------------------------------------------------------------
License
    This file is part of dtOO.

    dtOO is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the LICENSE.txt file in the
    dtOO root directory for more details.

    You should have received a copy of the License along with dtOO.

\*---------------------------------------------------------------------------*/

#include "analyticGeometry.h"

#include <attributionHeaven/pointGeometryDist.h>
#include <boost/assign/list_of.hpp>
#include <gslMinFloatAttr.h>
#include <interfaceHeaven/calculationTypeHandling.h>
#include <interfaceHeaven/staticPropertiesHandler.h>
#include <limits>
#include <logMe/logMe.h>
#include <progHelper.h>

namespace dtOO {
dtReal analyticGeometry::_deltaPer =
  staticPropertiesHandler::getInstance()->getOptionFloat(
    "analyticGeometry_deltaPer"
  );

analyticGeometry::analyticGeometry() : labelHandling()
{
  _resU = staticPropertiesHandler::getInstance()->getOptionInt(
    "geometry_render_resolution_u"
  );
  _resV = staticPropertiesHandler::getInstance()->getOptionInt(
    "geometry_render_resolution_v"
  );
  _resW = staticPropertiesHandler::getInstance()->getOptionInt(
    "geometry_render_resolution_w"
  );
  _characteristicLength = -1.;
  _boundingBoxValue = -1.;
}

analyticGeometry::analyticGeometry(analyticGeometry const &orig)
  : labelHandling(orig)
{
  _resU = orig._resU;
  _resV = orig._resV;
  _resW = orig._resW;
  _characteristicLength = orig._characteristicLength;
  _boundingBox = orig._boundingBox;
  _boundingBoxValue = orig._boundingBoxValue;
}

analyticGeometry::~analyticGeometry() {}

dtPoint3 analyticGeometry::getPoint(dtReal const *const uvw) const
{
  dt__throwUnexpected(getPoint());
}

dtPoint3 analyticGeometry::getPointPercent(dtReal const *const uvw) const
{
  dtReal uvwP[dim()];
  dt__forFromToIndex(0, dim(), ii) { uvwP[ii] = val_percent(uvw[ii], ii); }

  return getPoint(uvwP);
}

::std::vector<dtVector3>
analyticGeometry::firstDerPercent(dtReal const *const uvw) const
{
  dtReal uvwP[dim()];
  dt__forFromToIndex(0, dim(), ii) { uvwP[ii] = val_percent(uvw[ii], ii); }
  return firstDer(uvwP);
}

void analyticGeometry::setRenderResolution(dtInt const &dir, dtInt const &value)
  const
{
  switch (dir)
  {
  case 0:
    _resU = value;
    return;
  case 1:
    _resV = value;
    return;
  case 2:
    _resW = value;
    return;
  default:
    dt__throw(
      setRenderResolution(),
      << dt__eval(dir) << std::endl
      << "dir should be 0, 1 or 2."
    );
  }
}

int analyticGeometry::getRenderResolution(int const &dir) const
{
  switch (dir)
  {
  case 0:
    return _resU;
  case 1:
    return _resV;
  case 2:
    return _resW;
  default:
    dt__throw(
      getRenderResolution(),
      << dt__eval(dir) << std::endl
      << "dir should be 0, 1 or 2."
    );
  }
}

std::vector<dtVector3> analyticGeometry::firstDer(dtReal const *const uvw) const
{
  dtInt const nDim = this->dim();
  std::vector<dtReal> uvwP(nDim, std::numeric_limits<dtReal>::quiet_NaN());
  std::vector<dtVector3> dd(
    dim(),
    dtVector3(
      std::numeric_limits<dtReal>::quiet_NaN(),
      std::numeric_limits<dtReal>::quiet_NaN(),
      std::numeric_limits<dtReal>::quiet_NaN()
    )
  );
  dtReal const deltaPerInv = 1.0 - _deltaPer;
  dt__forFromToIndex(0, nDim, dim) { uvwP[dim] = percent_val(uvw[dim], dim); }

  dt__forFromToIndex(0, nDim, dim)
  {
    std::vector<dtReal> uvwP_h = uvwP;
    std::vector<dtReal> uvwP_l = uvwP;
    uvwP_h[dim] = uvwP_h[dim] + _deltaPer;
    uvwP_l[dim] = uvwP_l[dim] - _deltaPer;
    if (uvwP[dim] < _deltaPer)
    {
      uvwP_h[dim] = _deltaPer;
      uvwP_l[dim] = 0.0;
    }
    else if (uvwP[dim] > deltaPerInv)
    {
      uvwP_h[dim] = 1.0;
      uvwP_l[dim] = deltaPerInv;
    }
    dd[dim] = (getPointPercent(&uvwP_h[0]) - getPointPercent(&uvwP_l[0])) /
              (val_percent(uvwP_h[dim], dim) - val_percent(uvwP_l[dim], dim));
  }
  return dd;
}

void analyticGeometry::dump(void) const
{
  dt__info(
    dump(),
    << dt__eval(getLabel()) << std::endl
    << dt__eval(virtualClassName()) << std::endl
    << dumpToString()
  );
}

std::string analyticGeometry::dumpToString(void) const
{
  std::stringstream ss;

  ss << dt__dumpToString(<< "");

  return ss.str();
}

bool analyticGeometry::isTransformed(void) const { return false; }

bool analyticGeometry::isCompound(void) const { return false; }

vectorHandling<analyticGeometry const *> analyticGeometry::compoundInternal(void
) const
{
  return vectorHandling<analyticGeometry const *>();
}

std::pair<dtPoint3, dtPoint3> analyticGeometry::boundingBox(void) const
{
  if (_characteristicLength < 0.)
    updateBoundingBox();

  return _boundingBox;
}

dtReal analyticGeometry::boundingBoxValue(void) const
{
  if (_characteristicLength < 0.)
    updateBoundingBox();

  return _boundingBoxValue;
}

void analyticGeometry::updateBoundingBox(void) const
{
  dtReal uvwPoints[64][3] = {
    {0.00, 0.00, 0.00}, {0.30, 0.00, 0.00}, {0.60, 0.00, 0.00},
    {1.00, 0.00, 0.00}, {0.00, 0.30, 0.00}, {0.30, 0.30, 0.00},
    {0.60, 0.30, 0.00}, {1.00, 0.30, 0.00}, {0.00, 0.60, 0.00},
    {0.30, 0.60, 0.00}, {0.60, 0.60, 0.00}, {1.00, 0.60, 0.00},
    {0.00, 1.00, 0.00}, {0.30, 1.00, 0.00}, {0.60, 1.00, 0.00},
    {1.00, 1.00, 0.00}, //
    {0.00, 0.00, 0.30}, {0.30, 0.00, 0.30}, {0.60, 0.00, 0.30},
    {1.00, 0.00, 0.30}, {0.00, 0.30, 0.30}, {0.30, 0.30, 0.30},
    {0.60, 0.30, 0.30}, {1.00, 0.30, 0.30}, {0.00, 0.60, 0.30},
    {0.30, 0.60, 0.30}, {0.60, 0.60, 0.30}, {1.00, 0.60, 0.30},
    {0.00, 1.00, 0.30}, {0.30, 1.00, 0.30}, {0.60, 1.00, 0.30},
    {1.00, 1.00, 0.30}, //
    {0.00, 0.00, 0.60}, {0.30, 0.00, 0.60}, {0.60, 0.00, 0.60},
    {1.00, 0.00, 0.60}, {0.00, 0.30, 0.60}, {0.30, 0.30, 0.60},
    {0.60, 0.30, 0.60}, {1.00, 0.30, 0.60}, {0.00, 0.60, 0.60},
    {0.30, 0.60, 0.60}, {0.60, 0.60, 0.60}, {1.00, 0.60, 0.60},
    {0.00, 1.00, 0.60}, {0.30, 1.00, 0.60}, {0.60, 1.00, 0.60},
    {1.00, 1.00, 0.60}, //
    {0.00, 0.00, 1.00}, {0.30, 0.00, 1.00}, {0.60, 0.00, 1.00},
    {1.00, 0.00, 1.00}, {0.00, 0.30, 1.00}, {0.30, 0.30, 1.00},
    {0.60, 0.30, 1.00}, {1.00, 0.30, 1.00}, {0.00, 0.60, 1.00},
    {0.30, 0.60, 1.00}, {0.60, 0.60, 1.00}, {1.00, 0.60, 1.00},
    {0.00, 1.00, 1.00}, {0.30, 1.00, 1.00}, {0.60, 1.00, 1.00},
    {1.00, 1.00, 1.00}
  };

  if (dim() == 1)
  {
    dtInt const nPoints = 4;
    std::vector<dtPoint3> bb(nPoints);
    dt__forFromToIndex(0, nPoints, ii)
    {
      bb[ii] = getPointPercent(uvwPoints[ii]);
    }
    _boundingBox = dtLinearAlgebra::boundingBox(bb);
    _boundingBoxValue = dtLinearAlgebra::distance(bb[1], bb[0]) +
                        dtLinearAlgebra::distance(bb[2], bb[1]) +
                        dtLinearAlgebra::distance(bb[3], bb[2]);
  }
  else if (dim() == 2)
  {
    dtInt const nPoints = 16;
    std::vector<dtPoint3> bb(nPoints);
    dt__forFromToIndex(0, nPoints, ii)
    {
      bb[ii] = getPointPercent(uvwPoints[ii]);
    }
    _boundingBox = dtLinearAlgebra::boundingBox(bb);

    dtInt const I = 4;
    dtInt const J = 4;
    _boundingBoxValue = 0.0;
    dt__forFromToIndex(0, I - 1, ii)
    {
      dt__forFromToIndex(0, J - 1, jj)
      {
        dtInt const at0 = ii * J + jj;
        dtInt const at1 = (ii + 1) * J + jj;
        dtInt const at2 = ii * J + (jj + 1);
        dtInt const at3 = (ii + 1) * J + (jj + 1);
        _boundingBoxValue =
          _boundingBoxValue +
          dtLinearAlgebra::area(bb[at0], bb[at1], bb[at2], bb[at3]);
      }
    }
  }
  else if (dim() == 3)
  {
    dtInt const nPoints = 64;
    std::vector<dtPoint3> bb(nPoints);
    dt__forFromToIndex(0, nPoints, ii)
    {
      bb[ii] = getPointPercent(uvwPoints[ii]);
    }
    _boundingBox = dtLinearAlgebra::boundingBox(bb);

    dtInt const I = 4;
    dtInt const J = 4;
    dtInt const K = 4;
    _boundingBoxValue = 0.0;
    dt__forFromToIndex(0, I - 1, ii)
    {
      dt__forFromToIndex(0, J - 1, jj)
      {
        dt__forFromToIndex(0, K - 1, kk)
        {
          dtInt const at0 = ii * J * K + jj * K + kk;
          dtInt const at1 = (ii + 1) * J * K + jj * K + kk;
          dtInt const at2 = ii * J * K + (jj + 1) * K + kk;
          dtInt const at3 = (ii + 1) * J * K + (jj + 1) * K + kk;
          dtInt const at4 = ii * J * K + jj * K + (kk + 1);
          dtInt const at5 = (ii + 1) * J * K + jj * K + (kk + 1);
          dtInt const at6 = ii * J * K + (jj + 1) * K + (kk + 1);
          dtInt const at7 = (ii + 1) * J * K + (jj + 1) * K + (kk + 1);
          _boundingBoxValue = _boundingBoxValue + dtLinearAlgebra::volume(
                                                    bb[at0],
                                                    bb[at1],
                                                    bb[at2],
                                                    bb[at3],
                                                    bb[at4],
                                                    bb[at5],
                                                    bb[at6],
                                                    bb[at7]
                                                  );
        }
      }
    }
  }
  else
    dt__throwUnexpected(updateBoundingBox());

  _characteristicLength =
    dtLinearAlgebra::length(_boundingBox.first - _boundingBox.second);
}

std::vector<dtPoint3> analyticGeometry::cornerPoints(void) const
{
  //
  // 1D
  //
  if (dim() == 1)
  {
    dtInt nOne = 2;
    dtReal uvwOne[2] = {0.00, 1.00};

    std::vector<dtPoint3> bb(nOne);
    dt__forFromToIndex(0, nOne, ii)
    {
      bb[ii] = getPointPercent((dtReal *)&(uvwOne[ii]));
    }

    return bb;
  }
  //
  // 2D
  //
  else if (dim() == 2)
  {
    dtInt nTwo = 4;
    dtReal uvwTwo[4][2] = {
      {0.00, 0.00}, {0.00, 1.00}, {1.00, 0.00}, {1.00, 1.00}
    };

    std::vector<dtPoint3> bb(nTwo);
    dt__forFromToIndex(0, nTwo, ii)
    {
      bb[ii] = getPointPercent((dtReal *)&(uvwTwo[ii]));
    }

    return bb;
  }
  //
  // 3D
  //
  else if (dim() == 3)
  {
    dtInt nThree = 8;
    dtReal uvwThree[8][3] = {
      {0.00, 0.00, 0.00},
      {0.00, 0.00, 1.00},
      {0.00, 1.00, 0.00},
      {0.00, 1.00, 1.00},
      {1.00, 0.00, 0.00},
      {1.00, 0.00, 1.00},
      {1.00, 1.00, 0.00},
      {1.00, 1.00, 1.00}
    };

    std::vector<dtPoint3> bb(nThree);
    dt__forFromToIndex(0, nThree, ii)
    {
      bb[ii] = getPointPercent((dtReal *)&(uvwThree[ii]));
    }

    return bb;
  }
  else
    dt__throwUnexpected(updateBoundingBox());
}

bool analyticGeometry::reparam(dtPoint3 const &xyz, std::vector<dtReal> &uvw)
  const
{
  // convert initial guess to percent
  dt__forAllIndex(uvw, ii) { uvw[ii] = val_percent(uvw[ii], ii); }

  // initialize guesses and steps
  std::vector<std::vector<dtReal>> guesses;
  std::vector<dtReal> steps;
  if (this->dim() == 1)
  {
    guesses = std::vector<std::vector<dtReal>>(
      ::boost::assign::list_of
      // clang-format off
        (::boost::assign::list_of(0.50))
        (::boost::assign::list_of(0.75))
        (::boost::assign::list_of(0.25))
      // clang-format on
    );
    steps = std::vector<dtReal>(::boost::assign::list_of(0.001));
  }
  else if (this->dim() == 2)
  {
    guesses = std::vector<std::vector<dtReal>>(
      ::boost::assign::list_of
      // clang-format off
        (::boost::assign::list_of(0.50)(0.50))
        (::boost::assign::list_of(0.75)(0.50))
        (::boost::assign::list_of(0.25)(0.50))
        (::boost::assign::list_of(0.50)(0.75))
        (::boost::assign::list_of(0.75)(0.75))
        (::boost::assign::list_of(0.25)(0.75))
        (::boost::assign::list_of(0.50)(0.25))
        (::boost::assign::list_of(0.75)(0.25))
        (::boost::assign::list_of(0.25)(0.25))
      // clang-format on
    );
    steps = std::vector<dtReal>(::boost::assign::list_of(0.001)(0.001));
  }
  else if (this->dim() == 3)
  {
    guesses =
      std::
        vector<std::
                 vector<dtReal>>(::boost::assign::list_of
                                 // clang-format off
        (::boost::assign::list_of(0.50)(0.50)(0.50))
        (::boost::assign::list_of(0.75)(0.50)(0.50))
        (::boost::assign::list_of(0.25)(0.50)(0.50))
        (::boost::assign::list_of(0.50)(0.75)(0.50))
        (::boost::assign::list_of(0.75)(0.75)(0.50))
        (::boost::assign::list_of(0.25)(0.75)(0.50))
        (::boost::assign::list_of(0.50)(0.25)(0.50))
        (::boost::assign::list_of(0.75)(0.25)(0.50))
        (::boost::assign::list_of(0.25)(0.25)(0.50))
        (::boost::assign::list_of(0.50)(0.50)(0.25))
        (::boost::assign::list_of(0.75)(0.50)(0.25))
        (::boost::assign::list_of(0.25)(0.50)(0.25))
        (::boost::assign::list_of(0.50)(0.75)(0.25))
        (::boost::assign::list_of(0.75)(0.75)(0.25))
        (::boost::assign::list_of(0.25)(0.75)(0.25))
        (::boost::assign::list_of(0.50)(0.25)(0.25))
        (::boost::assign::list_of(0.75)(0.25)(0.25))
        (::boost::assign::list_of(0.25)(0.25)(0.25))
        (::boost::assign::list_of(0.50)(0.50)(0.25))
        (::boost::assign::list_of(0.75)(0.50)(0.25))
        (::boost::assign::list_of(0.25)(0.50)(0.25))
        (::boost::assign::list_of(0.50)(0.75)(0.25))
        (::boost::assign::list_of(0.75)(0.75)(0.25))
        (::boost::assign::list_of(0.25)(0.75)(0.25))
        (::boost::assign::list_of(0.50)(0.25)(0.25))
        (::boost::assign::list_of(0.75)(0.25)(0.25))
        (::boost::assign::list_of(0.25)(0.25)(0.25))
                                 // clang-format on
        );
    steps = std::vector<dtReal>(::boost::assign::list_of(0.001)(0.001)(0.001));
  }
  else
    dt__throwUnexpected(reparam());

  // initialize return vector; it an initial guess is present, prepend it to
  // the guesses vector; if not, set return value to nan
  if (uvw.empty())
  {
    uvw = std::vector<dtReal>(dim(), std::numeric_limits<dtReal>::quiet_NaN());
  }
  else
  {
    guesses.insert(guesses.begin(), uvw);
  }

  // perform minimization
  gslMinFloatAttr md(
    dt__pH(pointGeometryDist)(new pointGeometryDist(xyz, this)),
    guesses,
    steps,
    XYZTolerance(),
    1000
  );
  md.perform();

  // convert result from percent to parameter space
  std::vector<dtReal> const &uvw_percent = md.result();
  dt__forAllIndex(uvw_percent, ii)
  {
    uvw[ii] = val_percent(uvw_percent[ii], ii);
  }
  // return converged flag
  return md.converged();
}

dtReal analyticGeometry::characteristicLength(void) const
{
  if (_characteristicLength < 0.)
    updateBoundingBox();

  return _characteristicLength;
}

dtPoint3 analyticGeometry::getPoint(std::vector<dtReal> const &uvw) const
{
  return getPoint(&(uvw[0]));
}

dtReal analyticGeometry::XYZTolerance(void)
{
  return staticPropertiesHandler::getInstance()->getOptionFloat("xyz_resolution"
  );
}

bool analyticGeometry::inXYZTolerance(
  dtPoint3 const &p0,
  dtPoint3 const &p1,
  dtReal *const dist,
  bool output,
  dtReal inc
)
{
  dtReal xyzResolution = inc * analyticGeometry::XYZTolerance();

  dtVector3 distV = p0 - p1;
  *dist = dtLinearAlgebra::length(distV);
  if (*dist > xyzResolution)
  {
    if (output)
    {
      dt__warning(
        inXYZTolerance(),
        << dt__point3d(p0) << std::endl
        << dt__point3d(p1) << std::endl
        << dt__eval(*dist)
      );
    }
    return false;
  }

  return true;
}

bool analyticGeometry::inXYZTolerance(dtPoint3 const &p0, dtPoint3 const &p1)
{
  dtReal dist;
  return analyticGeometry::inXYZTolerance(p0, p1, &dist, false, 1.0);
}

bool analyticGeometry::inXYZTolerance(dtReal const dist, dtReal inc)
{
  dtReal xyzResolution = inc * analyticGeometry::XYZTolerance();

  if (dist > xyzResolution)
    return false;

  return true;
}

bool analyticGeometry::degenerated(void) const
{
  return inXYZTolerance(boundingBoxValue());
}

bool analyticGeometry::inUVWTolerance(dtPoint3 const &p0, dtPoint3 const &p1)
{
  dtReal uvwResolution =
    staticPropertiesHandler::getInstance()->getOptionFloat("uvw_resolution");

  dtVector3 dist = p0 - p1;
  if (sqrt(dist.squared_length()) > uvwResolution)
    return false;

  return true;
}

bool analyticGeometry::inUVWTolerance(dtPoint2 const &p0, dtPoint2 const &p1)
{
  dtReal uvwResolution =
    staticPropertiesHandler::getInstance()->getOptionFloat("uvw_resolution");

  dtVector2 dist = p0 - p1;
  if (sqrt(dist.squared_length()) > uvwResolution)
    return false;

  return true;
}

bool analyticGeometry::inUVWTolerance(dtReal const &p0, dtReal const &p1)
{
  dtReal uvwResolution =
    staticPropertiesHandler::getInstance()->getOptionFloat("uvw_resolution");

  dtReal dist = p0 - p1;
  if (fabs(dist) > uvwResolution)
    return false;

  return true;
}

bool analyticGeometry::equal(const analyticGeometry &other) const
{
  dt__throwIf(this->dim() != other.dim(), equal());

  std::vector<dtPoint3> thisCP = this->cornerPoints();
  std::vector<dtPoint3> otherCP = other.cornerPoints();

  std::vector<dtInt> matchCP;
  dt__forAllRefAuto(thisCP, aCP)
  {
    matchCP.push_back(dtLinearAlgebra::returnNearestPointIndexTo(aCP, otherCP));

    if (!inXYZTolerance(aCP, otherCP[matchCP.back()]))
      return false;
  }

  progHelper::removeBastardTwins(matchCP);
  if (matchCP.size() != thisCP.size())
    return false;

  return true;
}

bool analyticGeometry::equal(analyticGeometry const *const other) const
{
  return this->equal(*other);
}

dtReal analyticGeometry::val_percent(dtReal const &per, int const &dir) const
{
  if (isClosed(dir))
    return (getMin(dir) + (getMax(dir) - getMin(dir)) * per);

  return floatHandling::boundToRange(
    getMin(dir) + (getMax(dir) - getMin(dir)) * per, getMin(dir), getMax(dir)
  );
}

dtReal analyticGeometry::percent_val(dtReal const &val, int const &dir) const
{
  if (isClosed(dir))
    return (val - getMin(dir)) / (getMax(dir) - getMin(dir));

  return floatHandling::boundToRange(
    (val - getMin(dir)) / (getMax(dir) - getMin(dir)), 0.0, 1.0
  );
}

dt__C_addCloneForpVH(analyticGeometry);
} // namespace dtOO
