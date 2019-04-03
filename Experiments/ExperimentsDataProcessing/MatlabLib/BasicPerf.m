function [ P ] = BasicPerf(E, T, maxTime)
%BASICPERF Summary of this function goes here
%   Detailed explanation goes here


% On peut ajouter les composantes de perf de pr�cision et de rapidit�
% ind�pendemment,
% Puis multiplier par un coefficient g�n�ral de pr�cision (car les
% outliers / r�sultats tr�s faux doivent d�gager le mieux possible...)

precisionPerf = 1.0 - 2.0*E;
timePerf = 1 - T/(maxTime*1.5);

% performance qui somme et pond�re les perfs de temps et de pr�cision
P = precisionPerf + 0.70 * timePerf;
% clamp n�gatif
P = max(0, P);

% mise en facteur de l'erreur (qui reste la plus importante...)
P = 0.65 * P * (1.0 - 1.3*E) ;



% clamp
P = min(max(0, P), 1);

end

