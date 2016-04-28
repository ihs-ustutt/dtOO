classdef dtScalarMemField < dtScalarField
   properties (SetAccess = private, GetAccess = public)

   end
   methods
      function obj = dtScalarMemField(filename, coord, value, sf, q)
        %data = [ coord(:, 1); coord(:, 2); coord(:, 3); value(:,1); sf(:, 1); sf(:, 2); sf(:, 3); q(:, 1) ];
        %data = data';
        data(1:3,:) = coord(:, 1:3)';
        data(4,:) = value(:, 1:1)';
        data(5:7,:) = sf(:, 1:3)';
        data(8,:) = q(:, 1:1)';
        
        fid = fopen(filename,'w');
        fprintf(fid, '%16.8e,%16.8e,%16.8e,%16.8e,%16.8e,%16.8e,%16.8e,%16.8e\n', ...
          data ...
        );
        fclose(fid);
        obj@dtScalarField( filename );
      end     
   end
end