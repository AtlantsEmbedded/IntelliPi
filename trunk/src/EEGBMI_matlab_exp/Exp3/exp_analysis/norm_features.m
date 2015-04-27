%****************************************************************************************
% AUTHOR        : Frédéric Simard                                                       *
% DATE          : 04/15                                                                 *
% FILE          : norm_features.m                                                       *
% DESCRIPTION   : this function normalizes the features of a dataset. 4 options are     *
%                 available:                                                            *
%                 0: no normalization                                                   *
%                 1: rescale by dividing by max value, feature-wise                     *
%                 2: x' = (x - min)/(max-min), rescale between 0 and 1                  *
%                 3: ' = (x - mean)/(std_dev), to standard normal distribution          *
%                                                                                       *
% PARAMÈTRES :                                                                          *
%   - feature_vectors: the dataset to be normalized                                     *
%   - normalize_opt: option to select the normalization to be applied                   *
%                                                                                       *
% RETOURS :                                                                             *
%   - feature_vectors: the dataset normalized                                           *
%                                                                                       *
%****************************************************************************************
function [ feature_vectors ] = norm_features( feature_vectors, normalize_opt )

% select the normalization procedure, based on the
% option parameter
switch(normalize_opt)
    
    case 0
        % nothing
    case 1
        % divide by max value, feature-wise
        for ii=1:size(feature_vectors,2)
            feature_vectors(:,ii) = feature_vectors(:,ii)/max(feature_vectors(:,ii));
        end
        
    case 2
        % x' = (x - min)/(max-min), feature-wise
        for ii=1:size(feature_vectors,2)
            feature_vectors(:,ii) = (feature_vectors(:,ii)-min(feature_vectors(:,ii)))/(max(feature_vectors(:,ii))-min(feature_vectors(:,ii)));
        end
       
    case 3
        % x' = (x - mean)/(std_dev), feature-wise
        for ii=1:size(feature_vectors,2)
            feature_vectors(:,ii) = (feature_vectors(:,ii)-mean(feature_vectors(:,ii)))/std(feature_vectors(:,ii));
        end
end

end

