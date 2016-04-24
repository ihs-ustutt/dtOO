classdef dtDraftTube < dtState
   properties (SetAccess = private, GetAccess = public)
     wallShearField_
     uCylInletField_
     uCircumInletFieldCircAverage_
     uAxialInletFieldCircAverage_
     pTotInTime_
     pTotOutTime_
   end
   methods
      function obj = dtDraftTube(stateDir, prefix)
        % init state
        obj@dtState(stateDir, prefix)
        
        obj.wallShearField_ ...
        = ...
        dtScalarField( [obj.Path(), '/analyzeDraftTube_value.csv'] );
        obj.uCylInletField_ ...
        = ...
        dtVector3Field( [obj.Path(), '/volVectorPatchFieldRange_Ucyl.csv'] );
        
        [coord, value, weight] = obj.uCylInletField_.Field;
        
        [th, r, z] = cart2pol(coord(:,1), coord(:,2), coord(:,3));
        obj.uCircumInletFieldCircAverage_ ...
        = ...
        dtCircAverage(r, th, z, value(:,2), weight, 100);
        obj.uAxialInletFieldCircAverage_ ...
        = ...
        dtCircAverage(r, th, z, value(:,3), weight, 100);        
      
        obj.pTotInTime_ ...
        = ...
        dtScalarTime([obj.Path(), '/postProcessing/swakExpression_ptot_in_ma'], '%f %f');
        obj.pTotOutTime_ ...
        = ...
        dtScalarTime([obj.Path(), '/postProcessing/swakExpression_ptot_out_ma'], '%f %f');      
      end          
   end
end