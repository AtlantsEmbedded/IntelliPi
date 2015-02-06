function [ feature_vectors ] = preprocess_data( trials_data, baseline_data )

% compute the parameters of the fft
Fs = 220;
L = numel(trials_data{1}(:,1));
NFFT = 2^nextpow2(L); % Next power of 2 from length of y

% allocated memory
featvec_length = numel((1:NFFT/4+1));
feature_vectors = zeros(size(trials_data,1),featvec_length*4);

% run over all trials
for ii=1:numel(trials_data)
    
    Y = zeros(4,featvec_length);
    
    % compute the fft for each channel
    for jj=1:size(trials_data{1},2)

        data = trials_data{ii}(:,jj);
        data = data - mean(data);
        temp = fft(data,NFFT)/L;
        Y(jj,:) = abs(temp(1:NFFT/4+1))/L;
        
    end
    
%     % reshape the feature vector
     feature_vectors(ii,:) = reshape(Y',featvec_length*4,1);
    
%     Y = zeros(4,featvec_length);
%     
%     % compute the fft for the baseline
%     for jj=1:size(baseline_data{1},2)
% 
%         data = baseline_data{ii}(:,jj);
%           data = data-mean(data);
%         temp = fft(data,NFFT)/L;
%         Y(jj,:) = abs(temp(1:NFFT/4+1))/L;
%         
%     end
%     
%     % reshape the feature vector
%     feature_vectors(ii,:) = feature_vectors(ii,:)./reshape(Y',featvec_length*4,1)';
%     
end


end

