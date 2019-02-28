function [ outputMatrix ] = ExtendFirstAndLastValues( inputMatrix, lastTime )
%DUPLICATEFIRSTVALUETOTIMEZERO Summary of this function goes here
%   Detailed explanation goes here

% On met la premi�re ligne avec la m�me deuxieme colonne, 
% mais en premi�re colonne on force le temps z�ro (si �a
% n'est pas d�j� le cas)
if (inputMatrix(1,1) == 0)
    outputMatrix = inputMatrix;
else
    outputMatrix = [ 0 inputMatrix(1,2) ; inputMatrix];
end

% Last time : � d�placer si besoin
% (on ne sait pas si c'est ce jeu de donn�es qui a le plus long temps
% d'exp�rience)
if (inputMatrix(end,1) < lastTime)
    outputMatrix = [ outputMatrix ; lastTime outputMatrix(end,2) ];
else
    % rien
end


end

