
%
% N-FFT of a single 2N signal 
% (doesn't work)
%

N=2^7;
% create two real valued signal with known properties
xx = (0:N-1)/N;
signal_1 = sin(xx*2*pi*10)+2*sin(xx*2*pi*20);
signal_2 = sin(xx*2*pi*5)+2*sin(xx*2*pi*40);

% fft each of them
Xref_1 = fft(signal_1,N);
Xref_2 = fft(signal_2,N);
figure('name','reference')
hold on;
plot(abs(Xref_1),'color','b');
plot(abs(Xref_2),'color','r');


%%
% repeat, but this time we use a single fft operation


% form the signal
signal_cmpx = signal_1+1j*signal_2;

% compute the fft
X = fft(signal_cmpx,N);

% compute the split operation to recover individual X(n)
Xr_1(1) = real(X(1));
Xi_1(1) = 0;

Xr_2(1) = imag(X(1));
Xi_2(1) = 0;

Xr_1(N/2+1) = imag(X(N/2+1));
Xr_2(N/2+1) = imag(X(N/2+1));
Xi_1(N/2+1) = 0;
Xi_2(N/2+1) = 0;

for kk=1:N/2-1
    Xr_1(kk+1) = 0.5*(real(X(kk+1))+real(X(N-kk+1)));
    Xr_2(kk+1) = 0.5*(imag(X(kk+1))+imag(X(N-kk+1)));
    Xr_1(N-kk+1) = Xr_1(kk+1);
    Xr_2(N-kk+1) = Xr_2(kk+1);
    
    Xi_1(kk+1) = 0.5*(imag(X(kk+1))-imag(X(N-kk+1)));
    Xi_2(kk+1) = 0.5*(real(X(kk+1))-real(X(N-kk+1)));
    Xi_1(N-kk+1) = -Xi_1(kk+1);
    Xi_2(N-kk+1) = -Xi_2(kk+1);
end

X_1 = Xr_1+1j*Xi_1;
X_2 = Xr_2+1j*Xi_2;

figure('name','splitted')
hold on;
plot(abs(X_1),'color','b');
plot(abs(X_2),'color','r');


