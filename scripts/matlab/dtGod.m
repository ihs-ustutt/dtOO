classdef dtGod
   properties (Constant)
     g_ = 9.81;
     rho_ = 1000.;
   end
   methods (Static)
    function [g] = G()   
      g = dtGod.g_;
    end
    function [rho] = Rho()   
      rho = dtGod.rho_;
    end      
   end
end