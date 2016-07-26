classdef dtAnalyticFunction
   properties (SetAccess = private, GetAccess = public)
      filename_
      xDim_
      yDim_
      nP_
   end
   methods
      function obj = dtAnalyticFunction(filename)
        obj.filename_ = filename;

        if ( exist(obj.filename_, 'file') == 0);
          throw(  ...
            MException( ...
              'dtBladeCut:dtBladeCut', ...
              sprintf('> %s < is not a file.', obj.filename_) ...
            ) ...
          );
        end
        
        fid = fopen( obj.filename_ );
        tmpRead = fscanf(fid, '# xDim %d\n# yDim %d\n# nP %d %d %d\n');
        obj.xDim_ = tmpRead(1);
        obj.yDim_ = tmpRead(2);
        obj.nP_ = [ tmpRead(3) tmpRead(4) tmpRead(5) ];
        fclose(fid);
      end 
      function [ coord ] = Coord( obj )
        field = readGnu(obj.filename_, '%f %f %f');

        coord = zeros(size(field{1}, 1), 3);
        for i = 1:size(field{1}, 1)
          coord(i, 1:3) = field{1}(i,1:3);
        end
      end
   end
end