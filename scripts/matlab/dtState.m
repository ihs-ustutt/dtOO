classdef dtState
  properties (SetAccess = private, GetAccess = private)
    case_
    label_
    path_
  end
  methods
    function obj = dtState(stateDir, prefix)
      if ~isdir(stateDir)
        error([ stateDir, ' is not a directory']);
      end
      [pathstr, name, ext] = fileparts(stateDir);
      obj.path_ = stateDir;
      
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
  end
end