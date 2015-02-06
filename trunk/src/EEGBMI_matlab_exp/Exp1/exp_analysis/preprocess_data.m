function [ feature_vectors, f ] = preprocess_data( trials_data )


Fs = 220;
L = numel(trials_data{1}(:,1));
NFFT = 2^nextpow2(L); % Next power of 2 from length of y

featvec_length = numel((1:NFFT/2+1));
feature_vectors = zeros(70,featvec_length*4);
%feature_vectors = zeros(70,featvec_length);

for ii=1:numel(trials_data)
    Y = zeros(4,featvec_length);
    for jj=1:size(trials_data{1},2)

        data = trials_data{ii}(:,jj);
        data = data - mean(data);
        temp = fft(data,NFFT)/L;
        Y(jj,:) = abs(temp(1:NFFT/2+1))/L;
        
    end
    
    feature_vectors(ii,:) = reshape(Y',featvec_length*4,1);
    %feature_vectors(ii,:) = Y(2,:);
end

f = Fs/2*linspace(0,1,NFFT/2+1);

end

