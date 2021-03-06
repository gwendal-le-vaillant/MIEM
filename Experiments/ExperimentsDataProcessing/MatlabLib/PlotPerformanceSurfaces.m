function [] = PlotPerformanceSurfaces()
%PLOTPERFORMANCESURFACES Plots the surfaces corresponding to the
%computation methods for estimating the performance of a subject, depending
%on the error and 
%   Detailed explanation goes here


% sizes of the numeric mesh
N = 41;
maxError = 0.79;
N_step = maxError / (N-1);

maxDisplayTime = 35;
maxTime = 30;

M = 41;
M_step = maxDisplayTime / (M-1);

% norm-1 of the parametric error
% (values > 1 indicate very, very bad performance)
%
% AND total research time
% are both matrifces (in  a meshgrid)
[parametricError, searchTime] = meshgrid(0:N_step:N_step*(N-1), 0:M_step:M_step*(M-1));

% pre-allocation of matrices that represent surfaces
altPerf = zeros(M,N);

basicPerf = zeros(M,N);

% actual computation of the surfaces
for i=1:M % searchTime
    for j=1:N %  paramError
        
        altPerf(i,j) = AlternativePerf(parametricError(i,j), searchTime(i,j), maxTime);
        
        basicPerf(i,j) = BasicPerf(parametricError(i,j), searchTime(i,j), maxTime);
        
    end
end


% plotS
figure();

subplot(1,2,1);
surf(parametricError, searchTime, basicPerf);
title('In-Game performance function');
xlabel('Error E');
ylabel('Search duration T [s]');
ylim( [0 maxDisplayTime] );
zlabel('Performance P');
%view(0,90); % top view
view(135,45);
pbaspect([1 1 1]);

subplot(1,2,2);
surf(parametricError, searchTime, altPerf);
title('Basic performance function');
xlabel('Error E');
ylabel('Search duration T [s]');
ylim( [0 maxDisplayTime] );
zlabel('Performance P');
%view(0,90); % top view
view(135,45);
pbaspect([1 1 1]);


end

