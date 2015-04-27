%****************************************************************************************
% AUTHOR        : Frédéric Simard                                                       *
% DATE          : 04/15                                                                 *
% FILE          : bandsplit_data.m                                                      *
% DESCRIPTION   : Perform the eeg band filters on a data set of format                  *
%                 nb_trials x nb_channels x time_series. The results is stored as a     *
%                 vector of format nb_trials x nb_features, in which the features are   *
%                 the rms of each bands and of each channels concatenated one next to   *
%                 the other.                                                            *
%                                                                                       *
% PARAMÈTRES :                                                                          *
%   - trials_data: cell array of length nb_trials. Each cell contains a matrix of       *
%                  b_channels x time_series data                                        *
%                                                                                       *
% RETOURS :                                                                             *
%   - feature_vectors: nb_trials x nb_features array                                    *
%                                                                                       *
%****************************************************************************************
function [ feature_vectors ] = bandsplit_data( trials_data )

% parameters of the signal (Muse device)
fs = 220; % sampling freq
nb_channels = 4; % nb of channels (on the Muse)

order = 2; % order of the filter
nb_freq_bands = 5; % number of frequency bands (delta, theta, alpha, beta, gamma)

window_size = size(trials_data{1},1);

% create the filters (butterworth, bandpass)
[ filter_array] =  bandpass_eeg_filters(order, fs);
% allocated memory for feature vectors
feature_vectors = zeros(numel(trials_data), nb_freq_bands*nb_channels);

% for each trial
for ii=1:numel(trials_data)
    
	% temporary array to keep the features for this trial
    temp_array = zeros(nb_freq_bands,nb_channels);
    
    % for each channels
    for jj=1:size(trials_data{ii},2)
        
		% for freq bands
        for kk=1:nb_freq_bands
            
			% make a local copy of the signal
            x = trials_data{ii}(:,jj); 
			% double the length to skip initial ringing (patch, but legit)
            x = [x; x]; 
			x = x-mean(x);
			
            % apply the filter
            for i=1:order
				x = filter(squeeze(filter_array(kk,1,i,:)),squeeze(filter_array(kk,2,i,:)),x);
            end
			
            % compute RMS power of the samples within the window
            temp_array(kk,jj) = mean((x(end-(window_size-1):end)).^2);
        end
    end
    
    % fit in feature array (nb_trials:nb_bands*nb_channels)
    feature_vectors(ii,:) = reshape(temp_array,nb_freq_bands*nb_channels,1);
end



end

