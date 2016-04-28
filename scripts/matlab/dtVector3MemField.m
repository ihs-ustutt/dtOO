classdef dtVector3MemField < dtVector3Field
   properties (SetAccess = private, GetAccess = public)

   end
   methods
      function obj = dtVector3MemField(filename, coord, value, sf, q)
        %data = [ coord(:, 1); coord(:, 2); coord(:, 3); value(:,1); sf(:, 1); sf(:, 2); sf(:, 3); q(:, 1) ];
        %data = data';
        data(1:3,:) = coord(:, 1:3)';
        data(4:6,:) = value(:, 1:3)';
        data(7:9,:) = sf(:, 1:3)';
        data(10,:) = q(:, 1:1)';
        
        fid = fopen(filename,'w');
        fprintf(fid, '%16.8e,%16.8e,%16.8e,%16.8e,%16.8e,%16.8e,%16.8e,%16.8e,%16.8e,%16.8e\n', ...
          data ...
        );
        fclose(fid);
        obj@dtVector3Field( filename );
      end     
   end
end