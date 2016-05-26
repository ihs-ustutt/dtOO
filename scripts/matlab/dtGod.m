classdef (Sealed) dtGod < handle
   properties (SetAccess = private, GetAccess = public)
     g_;
     rho_;
   end
   methods (Access = private)
      function obj = dtGod
      end
   end
   methods (Static)
     function singleObj = Create( varargin )
       persistent localObj
       if isempty(localObj) || ~isvalid(localObj)
         if ( length(varargin)==0)
           localObj = dtGod;              
           localObj.g_ = 9.81;
           localObj.rho_ = 1000.;            
         elseif ( length(varargin)==2)
           localObj = dtGod;              
           localObj.g_ = varargin{1};
           localObj.rho_ = varargin{2};    
         else
           throw(  MException( 'dtGod:Create', 'varargin is not 0 or 2.' ) );              
          end
          fprintf('Create new dtGod with g = %f and rho = %f\n', localObj.g_, localObj.rho_);
       end
       singleObj = localObj;
     end
     function [] = Murder()
       thisGod = dtGod.Create();
       if ~isempty(thisGod) && isvalid(thisGod)         
         fprintf('Murder an existing dtGod\n');
       end
       delete(thisGod);         
     end      
     function [g] = G()   
       g = dtGod.Create().g_;
     end
     function [rho] = Rho()   
       rho = dtGod.Create().rho_;
     end      
   end
end