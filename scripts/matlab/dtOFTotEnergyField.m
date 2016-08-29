classdef dtOFTotEnergyField < dtScalarMemField
   properties (SetAccess = private, GetAccess = public)

   end
   methods
      function obj = dtOFTotEnergyField(filename, pres, offset, vel)
        coord = pres.coord_;
        value ...
        = ...
        (pres.value_ + offset) / dtGod.G ...
        + ...
        dt.Mag( vel.value_ ) .* dt.Mag( vel.value_ ) / (2.*dtGod.G);
        sf = pres.sf_;
        q = pres.q_;
        
        obj@dtScalarMemField( filename, coord, value, sf, q );
      end     
   end
end