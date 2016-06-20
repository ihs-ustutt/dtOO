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
%           throw(  ...
%             MException( ...
%               'dtState:dtState', ...
%               sprintf('> %s < is not a directory.', stateDir) ...
%             ) ...
%           );
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
  end
end