function [ X ] = fft_matlab( x, N)
%
% This file implements the Cooley–Tukey algorithm for fft based
% on the description found on the wiki page (see link below).
%
% http://en.wikipedia.org/wiki/Cooley%E2%80%93Tukey_FFT_algorithm
%
if N==1
    X = x;
else
    
    temp = fft_matlab(x(1:2:end), N/2);
    X(1:N/2) = temp;
    
    temp = fft_matlab(x(2:2:end), N/2);
    X(N/2+1:N) = temp;
    
    for k=1:(N/2)
       temp = X(k);
       X(k) = temp + exp(-2*pi*1i*k/N) * X(k+N/2);
       X(k+N/2) = temp - exp(-2*pi*1i*k/N) * X(k+N/2);
    end
end

end

