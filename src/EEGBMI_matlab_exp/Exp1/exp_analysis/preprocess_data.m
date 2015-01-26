function [ feature_vectors ] = preprocess_data( trials_data )


Fs = 220;
L = numel(trials_data{1}(:,1));
NFFT = 2^nextpow2(L); % Next power of 2 from length of y

featvec_length = numel((1:2:NFFT/4+1));
feature_vectors = zeros(70,featvec_length*4);

for ii=1:numel(trials_data)
    Y = zeros(4,featvec_length);
    for jj=1:size(trials_data{1},2)

        data = trials_data{ii}(:,jj);
        temp = fft(data,NFFT)/L;
        Y(jj,:) = abs(temp(1:2:NFFT/4+1))/L;
        
    end
    
    feature_vectors(ii,:) = reshape(Y',featvec_length*4,1);
end


end

