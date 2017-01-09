classdef dtScalarSpace
   properties (SetAccess = private, GetAccess = public)
      filename_
   end
   methods
      function obj = dtScalarSpace(filename)
        obj.filename_ = filename;

        if ( exist(obj.filename_, 'file') == 0);
          throw(  ...
            MException( ...
              'dtVector3Space:dtVector3Space', ...
              sprintf('> %s < is not a file.', obj.filename_) ...
            ) ...
          );
        end
      end 
      function [coord, value, v] = Space( obj )
        field = readGnu(obj.filename_, '%f,%f,%f,%f,%f');

        coord = zeros(size(field{1}, 1), 3);
        value = zeros(size(field{1}, 1), 1);
        v = zeros(size(field{1}, 1), 1);
        for i = 1:size(field{1}, 1)
          coord(i, 1:3) = field{1}(i,1:3);
          value(i, 1) = field{1}(i,4);
          v(i, 1) = field{1}(i,5);
        end
      end
   end
end