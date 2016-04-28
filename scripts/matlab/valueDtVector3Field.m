classdef valueDtVector3Field < dtVector3Field
   properties (SetAccess = private, GetAccess = public)
      coord_
      value_
      sf_
      q_
      mag_
      A_
      Q_
      intSf_
      intQSf_      
      trf_
   end
   methods
      function obj = valueDtVector3Field( initField )
        obj@dtVector3Field( initField.filename_ );

        [obj.coord_, obj.value_, obj.sf_, obj.q_] = obj.Field();

        obj.mag_ = dt.Mag( obj.value_ );        
             
        obj.A_ = dt.SumMag( obj.sf_ );
        obj.Q_ = dt.SumMag( obj.q_ );
        for i=1:3
          obj.intSf_(i) = dt.Sum( obj.value_(:,i) .* dt.Mag(obj.sf_) );
          obj.intQSf_(i) = dt.Sum( obj.value_(:,i) .* obj.q_ );    
        end
      end 
      function obj = Cyl( obj )
        obj.trf_ = dtCylTrf( obj.coord_ );
      end
      function [ r ] = R( obj )
        if( ~isa(obj.trf_,'dtCylTrf') ); error('trf_ is not a dtCylTrf.'); end
        r = obj.trf_.T1();
      end
%       function [ rvec ] = RVec( obj )
%         if( ~isa(obj.trf_,'dtCylTrf') ); error('trf_ is not a dtCylTrf.'); end
%         rvec = zeros( size(obj.trf_.T1, 1), 3 );
%         rvec(:,1) =  obj.trf_.T1 .* cos(obj.trf_.T2);
%         rvec(:,2) =  obj.trf_.T1 .* sin(obj.trf_.T2);
%       end        
      function [ phi ] = Phi( obj )
        if( ~isa(obj.trf_,'dtCylTrf') ); error('trf_ is not a dtCylTrf.'); end
        phi = obj.trf_.T2();
      end        
      function [ z ] = Z( obj )
        if( ~isa(obj.trf_,'dtCylTrf') ); error('trf_ is not a dtCylTrf.'); end
        z = obj.trf_.T3();
      end       
   end             
end