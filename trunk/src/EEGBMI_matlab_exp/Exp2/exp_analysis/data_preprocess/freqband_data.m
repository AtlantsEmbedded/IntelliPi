function [ feature_vectors ] = freqband_data( trials_data, baseline_data )

fs = 220;
freq_bands = {'delta','theta','alpha','beta','gamma','mu'};

featvec_length = numel(freq_bands);
feature_vectors = zeros(size(trials_data,1),featvec_length*4);

% process all trials
for ii=1:numel(trials_data)

    Y = zeros(4,featvec_length);
    
    % compute the fft for each channel
    for jj=1:size(trials_data{1},2)
        for kk=1:numel(freq_bands)
            
            [ freq, bandwidth ] = get_freq_band_info( freq_bands{kk} );
            data = trials_data{ii}(:,jj);
            data = data-mean(data);
            [ data_cplx ] = filter_eeg_signal( data, freq, bandwidth );
            Y(jj,kk) = mean(abs(data_cplx));
        end
    end
    
    % reshape the feature vector
    feature_vectors(ii,:) = reshape(Y',featvec_length*4,1);
    
    if nargin>1
        Y = zeros(4,featvec_length);

        % compute the fft for each channel
        for jj=1:size(trials_data{1},2)
            for kk=1:numel(freq_bands)

                [ freq, bandwidth ] = get_freq_band_info( freq_bands{kk} );
                data = baseline_data{ii}(:,jj);
                data = data-mean(data);
                [ data_cplx ] = filter_eeg_signal( data, freq, bandwidth );
                Y(jj,kk) = mean(abs(data_cplx));
            end
        end
        
        % reshape the feature vector
        feature_vectors(ii,:) = feature_vectors(ii,:) - reshape(Y',featvec_length*4,1)';
    end
    
end


end

