classdef dtValueVec3d2d < dtAnalyticFunction
   properties (SetAccess = private, GetAccess = public)
      coord_
   end
   methods
      function [ obj ] = dtValueVec3d2d( initFunction )
        obj@dtAnalyticFunction( initFunction.filename_ );

        obj.coord_ = obj.Coord();
      end 
      function [ xyz ] = ConstW( obj, num )
        xyz = zeros(obj.nP_(1), 3);
        for i=1:obj.nP_(1)
          xyz(i, 1:3) = obj.coord_( i + obj.nP_(1) * (num-1), 1:3 );
        end
      end
   end             
end