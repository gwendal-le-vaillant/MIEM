function [ P ] = BasicPerf(E, T, maxTime)
%BASICPERF Summary of this function goes here
%   Detailed explanation goes here


% On peut ajouter les composantes de perf de pr�cision et de rapidit�
% ind�pendemment,
% Puis multiplier par un coefficient g�n�ral de pr�cision (car les
% outliers / r�sultats tr�s faux doivent d�gager le mieux possible...)

precisionPerf = (1.0 - 2.0*E);
timePerf = (1 - T/(maxTime*1.0));
P = 0.6 * (precisionPerf + timePerf);
% clamp n�gatif
P = max(0, P);

P = P * (1.0 - 1.2*E) ;


% clamp
P = min(max(0, P), 1);

end

