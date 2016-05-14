classdef dtOFPotEnergyField < dtScalarMemField
   properties (SetAccess = private, GetAccess = public)

   end
   methods
      function obj = dtOFPotEnergyField(filename, pres, offset)
        coord = pres.coord_;
        value = (pres.value_ + offset) / dtGod.G;
        sf = pres.sf_;
        q = pres.q_;
        
        obj@dtScalarMemField( filename, coord, value, sf, q );
      end     
   end
end