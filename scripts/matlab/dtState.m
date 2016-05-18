classdef dtState
  properties (SetAccess = private, GetAccess = private)
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
          throw(  ...
            MException( ...
              'dtState:dtState', ...
              sprintf('> %s < is not a directory.', stateDir) ...
            ) ...
          );
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
end