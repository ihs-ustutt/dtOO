classdef dtVector3Space
   properties (SetAccess = private, GetAccess = public)
      filename_
   end
   methods
      function obj = dtVector3Space(filepattern)
        obj.filename_ = dtFile( filepattern ).FileName();
      end 
      function [coord, value, v] = Space( obj )
        field = readGnu(obj.filename_, '%f,%f,%f,%f,%f,%f,%f');

        coord = zeros(size(field{1}, 1), 3);
        value = zeros(size(field{1}, 1), 3);
        v = zeros(size(field{1}, 1), 1);
        for i = 1:size(field{1}, 1)
          coord(i, 1:3) = field{1}(i,1:3);
          value(i, 1:3) = field{1}(i,4:6);
          v(i, 1) = field{1}(i,7);
        end
      end
   end
end