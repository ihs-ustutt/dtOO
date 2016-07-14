classdef dtState
  properties (SetAccess = private, GetAccess = public)
    case_
    label_
    path_
    ok_
    prefix_
  end
  methods
    function obj = dtState(stateDir, prefix)
        obj.ok_ = 1;
        if (exist(stateDir, 'dir') == 0);
          obj.ok_ = 0;
          fprintf('> %s < is not a directory.\n', stateDir);          
          fprintf('Create failed state.\n');          
        end
      [pathstr, name, ext] = fileparts(stateDir);
      obj.path_ = stateDir;
      obj.prefix_ = prefix;
      if isempty(prefix)
        pos = findstr('_', name);
        obj.case_ = name(1:pos(end)-1);
        obj.label_ = name(pos(end)+1:end);        
      else
        pos = findstr(prefix, name);
        obj.case_ = name(1:pos-2);
        obj.label_ = name(pos:end);
      end
    end
    function str = Path(obj)
      str = obj.path_;
    end    
    function str = Label(obj)
      str = obj.label_;
    end        
    function str = Case(obj)
      str = obj.case_;
    end
    function str = Prefix(obj)
      str = obj.prefix_;
    end    
    function obj = MakeFail(obj)
      obj.ok_ = 0;
    end
    function [ ok ] = Ok( obj )
% Ok Check function.
%   ok : Return 1 if is ok, otherwise 0
      ok = obj.ok_;
    end         
  end
  methods (Static)
    function str = GiveStateDirectoyName(caseName, prefixStr, number)
      str = caseName;
      if ~isempty(prefixStr)
        str = strcat(str, '_', prefixStr);
      end
      str = strcat(str, '_', num2str(number));
    end    
    function ind = GiveIndex(str)
      oc = strfind(str, '_');
      if ~isempty(oc)
        ind = str2double( str( oc(end)+1:end) );
      else
        ind = str2double( str );
      end
    end
    function [ state, successLabel, failLabel ] ...
    = ...
    CheckSortUnify( state, successLabel, failLabel )
% CheckSortUnify Check state vector by calling function Ok and make
%   successLabel vector and failLabel vector unique. successLabel vector is
%   sorted in the same order as state vector.
      %
      % call Ok function
      %
      markToDel = [];
      for i=1:length(state)
        ok = state(i).Ok();
        if ~ok
          fprintf( 'state( %d ) [ %s ] : Ok = %d\n', i, class(state(i)), ok );

          successMember = find( ismember(successLabel, state(i).Label)==1 );
      
          if ( size(successMember) ); 
            successLabel( successMember ) = [];
            failLabel{end+1} = state(i).Label;
            markToDel(end+1) = i;
          end;
        end
      end
      state( markToDel ) = [];
      
      %
      % unify
      %
      failLabel = unique( failLabel );
      successLabel = unique( successLabel );
      
      %
      % correct ordering
      %
      for i=1:length(state)
        successLabel{i} = state(i).Label();
      end      
    end
    function [ has ] = Contains(stateLabelVector, state)
% Contains Check if stateLabelVector contains state. Return 1 if
%   state is part of stateLabelVector, otherwise return 0.
      has = 0;
      ind = find( ismember(stateLabelVector, state.Label() )==1 );
      if (~isempty(ind))
        has = 1;
      end
    end      
    function [ stateLabelVector ] = Remove(stateLabelVector, state)
% Remove Remove state in stateLabelVector if present.
      ind = find( ismember(stateLabelVector, state.Label() )==1 );
      if ( ~isempty(ind) )
        stateLabelVector( ind ) = [];
      end
    end     
  end
end