classdef dtOFKinEnergyMemField < dtScalarMemField
   properties (SetAccess = private, GetAccess = public)

   end
   methods
      function obj = dtOFKinEnergyMemField(filename, vel)
        coord = vel.coord_;
        value = dt.Mag( vel.value_ ) .* dt.Mag( vel.value_ ) / (2.*dtGod.G);
        sf = vel.sf_;
        q = vel.q_;
        
        obj@dtScalarMemField( filename, coord, value, sf, q );
      end     
   end
end