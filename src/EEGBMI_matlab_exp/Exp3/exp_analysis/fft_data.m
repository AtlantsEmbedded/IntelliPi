%****************************************************************************************
% AUTHOR        : Frédéric Simard                                                       *
% DATE          : 04/15                                                                 *
% FILE          : fft_data.m                                                            *
% DESCRIPTION   : Perform the fast fourier transform on a data set of format            *
%                 nb_trials x nb_channels x time_series. The results is stored as a     *
%                 vector of format nb_trials x nb_features, in which the features are   *
%                 the fft samples of each channels concatenated one next to the other.  *
%                                                                                       *
%                 Features are then normalize according to the selected option.         *
%                 See norm_features.m for details                                       *
%                                                                                       *
% PARAMÈTRES :                                                                          *
%   - trials_data: cell array of length nb_trials. Each cell contains a matrix of       *
%                  b_channels x time_series data                                        *
%                                                                                       *
% RETOURS :                                                                             *
%   - feature_vectors: nb_trials x nb_features array                                    *
%                                                                                       *
%****************************************************************************************
function [ feature_vectors ] = fft_data( trials_data )

% compute the parameters of the fft
Fs = 220;
L = numel(trials_data{1}(:,1));
NFFT = 2^nextpow2(L); % Next power of 2 from length of y

% allocate memory
featvec_length = numel((1:NFFT/4+1));
feature_vectors = zeros(size(trials_data,1),featvec_length*4);

% run over all trials
for ii=1:numel(trials_data)
    
    Y = zeros(4,featvec_length);
    
    % compute the fft for each channel
    for jj=1:size(trials_data{1},2)

        data = trials_data{ii}(:,jj);
        
        % remove the average of the signal (DC component)
        data = data - mean(data);
        
        % Normalize the amplitude of the fft data points
        temp = fft(data,NFFT)/L;
        
        % cut the reflexion in the fft vector
        Y(jj,:) = abs(temp(1:NFFT/4+1));
        
    end
    
	% reshape the matrix (freq x channels) into a one-dim vector
    feature_vectors(ii,:) = reshape(Y',featvec_length*4,1);
    
end


end

