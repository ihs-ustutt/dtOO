classdef dtValueVector3Space < dtVector3Space
   properties (SetAccess = private, GetAccess = public)
      coord_
      value_
      v_
      mag_
      V_
   end
   methods
      function obj = dtValueVector3Space( initField )
        obj@dtVector3Space( initField.filename_ );

        [obj.coord_, obj.value_, obj.v_] = obj.Space();

        obj.mag_ = dt.Mag( obj.value_ );        
             
        obj.V_ = dt.SumMag( obj.v_ );
      end 
   end             
end