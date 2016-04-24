classdef dtScalarField
   properties (SetAccess = private, GetAccess = public)
      filename_
   end
   methods
      function obj = dtScalarField(filename)
        obj.filename_ = filename;
        if (exist(obj.filename_) ~= 2);
          error(sprintf('exist() of file %s does not return 2', obj.filename_));
        end
      end 
      function [coord, value, weight] = Field( obj )
        field = readGnu(obj.filename_, '%f,%f,%f,%f,%f');

        coord = zeros(size(field{1}, 1), 3);
        value = zeros(size(field{1}, 1), 1);
        weight = zeros(size(field{1}, 1), 1);
        for i = 1:size(field{1}, 1)
          coord(i, 1:3) = field{1}(i,1:3);
          value(i, 1) = field{1}(i,4);
          weight(i, 1) = field{1}(i,5);
        end
      end       
   end
end