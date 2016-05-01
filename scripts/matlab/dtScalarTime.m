classdef dtScalarTime
   properties (SetAccess = private, GetAccess = public)
      dirname_
      timeDirs_
      label_
      format_
      time_
      value_
      nValues_
   end
   methods
      function obj = dtScalarTime(dirname, format)
        obj.dirname_ = dirname;
        obj.format_ = format;
        if (exist(obj.dirname_, 'dir') == 0);
          throw(  ...
            MException( ...
              'dtScalarTime:dtScalarTime', ...
              sprintf('> %s < is not a file.', obj.dirname_) ...
            ) ...
          );
        end
        
        dirs = dir(obj.dirname_);
        for i=1:size(dirs,1)
          if ~isempty( str2num(dirs(i).name) )
            obj.timeDirs_(end+1) = str2num(dirs(i).name);
          end
        end
        obj.timeDirs_ = sort( obj.timeDirs_ );
        
        for i=1:size(obj.timeDirs_, 2)
          inTimeDirs = dir( [obj.dirname_,'/', num2str(obj.timeDirs_(i))] );
          
          for j=1:size(inTimeDirs,1)
            if ( inTimeDirs(j).isdir ); continue; end;
            if ( isempty( obj.label_ ) )
              obj.label_ = inTimeDirs(j).name;
            end
            
            if ( ~strcmp(obj.label_, inTimeDirs(j).name) )
              throw( ...
                MException( ...
                  'dtScalarTime:dtScalarTime', ...
                  'File and label do not match.' ...
                ) ...
              );              
            end
            
            data ...
            = ...
            readGnu( ...
              [obj.dirname_,'/', num2str(obj.timeDirs_(i)), '/', obj.label_], obj.format_ ...
            );
          end
          obj.time_ = [ obj.time_; data{1}(:,1) ];
          obj.value_ = [ obj.value_; data{1}(:,2) ];
        end
        
        obj.nValues_ = size( obj.value_, 1);
      end
      
      function [val] = MeanLast( obj, nAverageSteps )
        val = mean( obj.value_(obj.nValues_ - nAverageSteps:end) );
      end       
      function [val] = MinLast( obj, nAverageSteps )
        val = min( obj.value_(obj.nValues_ - nAverageSteps:end) );
      end             
      function [val] = MaxLast( obj, nAverageSteps )
        val = max( obj.value_(obj.nValues_ - nAverageSteps:end) );
      end              
   end
end