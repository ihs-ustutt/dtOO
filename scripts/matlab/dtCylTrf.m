classdef dtCylTrf
   properties (SetAccess = private, GetAccess = public)   
      r_
      phi_
      z_
   end
   methods
      function obj = dtCylTrf( coord )
        [obj.phi_(:,1), obj.r_(:,1), obj.z_(:,1)] ...
        = ...
        cart2pol( coord(:,1), coord(:,2), coord(:,3) );
      end
      function [ r ] = T1( obj )
        r = obj.r_(:,1);
      end
%       function [ r ] = RVec( obj )
%         if( obj.trf_~=1 ); error('Object is not transformed!'); end
%         r = obj.coord_(:,1:3);
%         r(:,3) = r(:,3) - obj.z_(:);
%       end        
      function [ phi ] = T2( obj )
        phi = obj.phi_(:,1);
      end        
      function [ z ] = T3( obj )
        z = obj.z_(:,1);
      end              
   end  
end