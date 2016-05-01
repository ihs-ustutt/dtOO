classdef dtScalarField
   properties (SetAccess = private, GetAccess = public)
      filename_
   end
   methods
      function obj = dtScalarField(filename)
        obj.filename_ = filename;
        if (exist(obj.filename_, 'file') == 0);
          throw(  ...
            MException( ...
              'dtScalarField:dtScalarField', ...
              sprintf('> %s < is not a file.', obj.filename_) ...
            ) ...
          );
        end
      end 
      function [coord, value, sf, q] = Field( obj )
        field = readGnu(obj.filename_, '%f,%f,%f,%f,%f,%f,%f,%f');

        coord = zeros(size(field{1}, 1), 3);
        value = zeros(size(field{1}, 1), 1);
        sf = zeros(size(field{1}, 1), 3);
        q = zeros(size(field{1}, 1), 1);
        for i = 1:size(field{1}, 1)
          coord(i, 1:3) = field{1}(i,1:3);
          value(i, 1) = field{1}(i,4);
          sf(i, 1:3) = field{1}(i,5:7);
          q(i, 1) = field{1}(i,8);
        end
      end       
   end
end