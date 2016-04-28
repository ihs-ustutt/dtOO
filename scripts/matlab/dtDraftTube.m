classdef dtDraftTube < dtState
   properties (SetAccess = private, GetAccess = public)
     wallShearAreaField_
     uCylInletField_
%      uCircumInletFieldCircAverage_
%      uAxialInletFieldCircAverage_
     uCylInCircAverage_
     pInCircAverage_
     uInletField_
     uOutletField_
     pInletField_
     pOutletField_     
     pTotInTime_
     pTotOutTime_
     sepBound_
     sepArea_
     minWallShear_
     maxWallShear_
     deltaHLoss_
     cpI_
     cp_
   end
   methods
      function obj = dtDraftTube(stateDir, prefix, sepBound)
        % init state
        obj@dtState(stateDir, prefix)

        %
        % tau
        %
        obj.wallShearAreaField_ ...
        = ...
        dtVector3Field( [obj.Path(), '/volVectorPatchFieldRange_wallShearStress_DT_WALL_wallShearStress.csv'] );
         
        %
        % cylindrical velocity
        %
        obj.uCylInletField_ ...
        = ...
        dtVector3Field( [obj.Path(), '/volVectorPatchFieldRange_Ucyl_DT_INLET_Ucyl.csv'] );
        
        %
        % velocity
        %
        obj.uInletField_ ...
        = ...
        dtVector3Field( [obj.Path(), '/volVectorPatchFieldRange_U_DT_INLET_U.csv'] );
        obj.uOutletField_ ...
        = ...
        dtVector3Field( [obj.Path(), '/volVectorPatchFieldRange_U_DT_OUTLET_U.csv'] );
        
        %
        % p
        %
        obj.pInletField_ ...
        = ...
        dtScalarField( [obj.Path(), '/volScalarPatchFieldRange_p_DT_INLET_p.csv'] );
        obj.pOutletField_ ...
        = ...
        dtScalarField( [obj.Path(), '/volScalarPatchFieldRange_p_DT_OUTLET_p.csv'] );

        %
        % total pressure
        %
        obj.pTotInTime_ ...
        = ...
        dtScalarTime([obj.Path(), '/postProcessing/swakExpression_ptot_in_ma'], '%f %f');
        obj.pTotOutTime_ ...
        = ...
        dtScalarTime([obj.Path(), '/postProcessing/swakExpression_ptot_out_ma'], '%f %f');
      
        %
        % transform u to cylindrical coordinates
        %
        ucyl = valueDtVector3Field( obj.uCylInletField_ ).Cyl();
        
        %
        % circumferential velocity (circumferential massflow average)
        %
        obj.uCylInCircAverage_ = dtCircAverage(ucyl, 100);
                  
        %
        % calculate separation area and set min and max shear
        %
        obj.sepBound_ = sepBound;
        tau = valueDtVector3Field(obj.wallShearAreaField_);
        obj.sepArea_ = sum( dt.Mag( tau.sf_( find(tau.mag_<obj.sepBound_) ) ) );
        obj.minWallShear_ = min(tau.mag_);
        obj.maxWallShear_ = max(tau.mag_);
         
        % calculate deltaHLoss
        obj.deltaHLoss_ ...
        = ...
        ( obj.pTotInTime_.MeanLast(200) - obj.pTotOutTime_.MeanLast(200) ) / dtGod.G;

        uIn = valueDtVector3Field( obj.uInletField_ );
%        uOut = valueDtVector3Field( obj.uOutletField_ );        
        pIn = valueDtScalarField( obj.pInletField_ ).Cyl;
        obj.pInCircAverage_ = dtCircAverage(pIn, 100);
        
        pOut = valueDtScalarField( obj.pOutletField_ );    
        % kinetic energy inlet
        e_uIn ...
        = ...
        valueDtScalarField( ...
          dtScalarMemField( ...
            [obj.Path(), '/dtScalarMField_e_uIn.csv'], ...
            uIn.coord_, ...
            (1/2) * dt.Mag( uIn.value_ ) .* dt.Mag( uIn.value_ ), ...
            uIn.sf_, uIn.q_ ...
          ) ...
        );
      
        % calculate ideal and real cp
        obj.cpI_ = 1. - pIn.A_^2/pOut.A_^2;        
        obj.cp_ ...
        = ...
        (pOut.intQSf_ - pIn.intQSf_) / e_uIn.intQSf_;
        
        % eulerHeadIn
        eulerHeadIn ...
        = ...
        valueDtVector3Field( ...
          dtVector3MemField( ...
            [obj.Path(), '/dtScalarMField_eulerHeadIn.csv'], ...
            uIn.coord_, ...
            cross([ ucyl.R .* cos(ucyl.Phi) ucyl.R .* sin(ucyl.Phi) ucyl.Z .* 0], uIn.value_), ...
            uIn.sf_, uIn.q_ ...
          ) ...
        );
      
      end
   end
end