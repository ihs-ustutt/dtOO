function [] = dtMassflowAverageUcyl( filename, nBins )
  ucyl = dtValueVector3Field( dtVector3Field(filename ) ).Cyl
  ucyl_ca = dtCircAverage( ucyl, nBins )
  [tDir, tName, tSuffix] = fileparts(filename);
  
  p = dtPlot( strcat(tName, '_r'), 'Arial', '10', '1', '1' );
  p.GiveSinglePlot( 8, 8, '$$ r \; [m] $$', '$$ u_r \; [ \frac{m}{s} ] $$');
  hold on;      
  grid on;
  plot( ucyl.R, ucyl.value_(:,1), 'k.', 'Color', p.lc_ );
  plot( ucyl_ca.binRMean_, ucyl_ca.binQ_(:,1), '-k', 'Linewidth', 2 );
  p.SaveJpg();
  
  p = dtPlot( strcat(tName, '_phi'), 'Arial', '10', '1', '1' );
  p.GiveSinglePlot( 8, 8, '$$ r \; [m] $$', '$$ u_\phi \; [ \frac{m}{s} ] $$');
  hold on;      
  grid on;
  plot( ucyl.R, ucyl.value_(:,2), 'k.', 'Color', p.lc_ );
  plot( ucyl_ca.binRMean_, ucyl_ca.binQ_(:,2), '-k', 'Linewidth', 2 );
  p.SaveJpg();
  
  p = dtPlot( strcat(tName, '_z'), 'Arial', '10', '1', '1' );
  p.GiveSinglePlot( 8, 8, '$$ r \; [m] $$', '$$ u_z \; [ \frac{m}{s} ] $$');
  hold on;      
  grid on;
  plot( ucyl.R, ucyl.value_(:,3), 'k.', 'Color', p.lc_ );
  plot( ucyl_ca.binRMean_, ucyl_ca.binQ_(:,3), '-k', 'Linewidth', 2 );
  p.SaveJpg();
       
  ucyl_ca.WriteCSV( ...
    ucyl, ...
    'Velocity Radial [ m s^-1 ], Velocity Circumferential [ m s^-1 ], Velocity Axial [ m s^-1 ]', ...
    '%16.8e, %16.8e, %16.8e' ...
  )
  
end