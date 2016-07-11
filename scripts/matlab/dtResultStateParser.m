classdef dtResultStateParser < dtStateParser
   properties (SetAccess = private, GetAccess = public)
     result_
     resultName_
     resultNameFig_
   end
   methods
      function obj = dtResultStateParser( filename )
        obj@dtStateParser(filename);
        
        nStates = length( obj.stateLabel_ );
        
        obj.result_ = zeros(nStates, 0);
        obj.resultName_ = {};
        obj.resultNameFig_ = {};        
      end
      function [ obj ] = AddResult( obj, states, resultName, resultNameFig, result )
        if ~isempty( find( ismember(obj.resultName_, resultName )==1 ) );
          return
        end
         %
         % set name and nameFig and extent y_
         %
         obj.resultName_{end+1} = resultName;
         obj.resultNameFig_{end+1} = resultNameFig;
         obj.result_(:,end+1) = zeros(size(obj.result_,1), 1);
         for i=1:length(states)
           cState = obj.StateIndex( states{i} );
           obj.result_(cState,end) = result(i);
         end
      end      
      function [ obj ] = RemoveResult( obj, resultName )
        toRemove = find( ismember(obj.resultName_, resultName )==1 );
        if ~isempty(toRemove)
          obj.resultName_(toRemove) = [];
          obj.resultNameFig_(toRemove) = [];
          obj.result_(:, toRemove) = [];          
        end
      end 
      function [ obj ] = RemoveAllResult( obj )
        obj.resultName_ = [];
        obj.resultNameFig_ = [];
        obj.result_ = [];                  
      end       
      function [ sH ] = ValueOfResult( obj, stateIn )
        state = {};
        if ~iscell(stateIn)
          state{1} = stateIn;
        else
          state = stateIn;
        end
        stateId = obj.StateIndex(state);
        sH = zeros( length(state), length( obj.resultName_ ) );
        for i=1:length( state )
          sH(i, :) = obj.result_(stateId(i), :);
        end
      end
      function [ sH ] = ValueOfOneResult( obj, state, resultName )
         stateId = obj.StateIndex(state);%find( ismember(state, obj.stateLabel_ )==1 );
         resultId = find( strcmp(obj.resultName_, resultName ) );
         sH = [];
%         for i=1:length( obj.resultName_ )
           sH = obj.result_(stateId, resultId);
%         end
      end
      function hasResult = HasResult(obj, result )
        hasResult = 1;
        if isempty( find( ismember(obj.resultName_, result )==1 ) )
          hasResult = 0;        
        end
      end      
      function resultNameFig = ResultNameFig(obj, result )
        resultPos = find( ismember(obj.resultName_, result )==1 );
        resultNameFig = obj.resultNameFig_{ resultPos };
      end            
   end
end