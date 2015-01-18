
%
% N-FFT of a single 2N signal 
% (doesn't work)
%


% create a real valued signal with known properties
xx = (0:2^7-1)/2^7;
signal = sin(xx*2*pi*10)+2*sin(xx*2*pi*20);

% fft it
result = fft(signal(1:2:end),2^6);
figure('name','reference')
subplot(211)
plot(signal,'color','b')
subplot(212)
plot(abs(result),'color','b');

% repeat, but this time we use the splitted approach
signal_split = signal(1:2:end)+1j*signal(2:2:end);

X = fft(signal_split,2^6);
figure('name','splitted')
subplot(211)
plot(signal,'color','b')
subplot(212)
plot(abs(X),'color','b');


% 	/* Rearrange the result X(k) to get the desired G(k)
% 	 * Equation: G(k) = X(k)*A(k) + X*(N-k)B(k)
% 	 *
% 	 * which can be expressed as:
% 	 * Gr(k) = Xr(k)Ar(k) - Xi(k)Ai(k) + Xr(N-k)Br(k) + Xi(N-k)Bi(k) 
% 	 * Gi(k) = Xi(k)Ar(k) + Xr(k)Ai(k) + Xr(N-k)Bi(k) - Xi(N-k)Br(k)
% 	 * with k = 0,1,...,N-1 and X(N) = X(0)
% 	 *
% 	 * Gr(N) = Xr(0) - Xi(0)
% 	 * Gi(N) = 0
% 	 *
% 	 * Symmetric part of the fft
% 	 * Gr(2N-k) = Gr(k)
% 	 * Gi(2N-k) = Gi(k)
% 	 *
% 	 * With Ar, Ai, Br and Bi equal to:
% 	 * Ar(k) = -sin(pi*k/N)
% 	 * Ai(k) = -cos(pi*k/N)
% 	 * Br(k) = sin(pi*k/N)
% 	 * Bi(k) = cos(pi*k/N)
% 	 * with k=0->N-1
% 	*/

%%
N=2^6;

for kk=0:N-1

    Ar(kk+1) = -sin(pi*kk/N);
    Ai(kk+1) = -cos(pi*kk/N);
    Br(kk+1) = sin(pi*kk/N);
    Bi(kk+1) = cos(pi*kk/N);

end

Gr = [];
Gi = [];
% 	 * Gr(k) = Xr(k)Ar(k) - Xi(k)Ai(k) + Xr(N-k)Br(k) + Xi(N-k)Bi(k) 
% 	 * Gi(k) = Xi(k)Ar(k) + Xr(k)Ai(k) + Xr(N-k)Bi(k) - Xi(N-k)Br(k)
kk=0;
Gr(kk+1) = real(X(kk+1))*Ar(kk+1) - imag(X(kk+1))*Ai(kk+1) + real(X(1))*Br(kk+1) + imag(X(1))*Bi(kk+1);
Gi(kk+1) = imag(X(kk+1))*Ar(kk+1) + real(X(kk+1))*Ai(kk+1) + real(X(1))*Bi(kk+1) - imag(X(1))*Br(kk+1);

for kk=1:N-1
    Gr(kk+1) = real(X(kk+1))*Ar(kk+1) - imag(X(kk+1))*Ai(kk+1);% + real(X((N+1)-kk))*Br(kk+1) + imag(X((N+1)-kk))*Bi(kk+1);
    %Gi(kk+1) = imag(X(kk+1))*Ar(kk+1) + real(X(kk+1))*Ai(kk+1) + real(X((N+1)-kk))*Bi(kk+1) - imag(X((N+1)-kk))*Br(kk+1);

    Gi(kk+1) = real(X((N+1)-kk))*Br(kk+1) + imag(X((N+1)-kk))*Bi(kk+1);
    %Gi(kk+1) = real(X((N+1)-kk))*Bi(kk+1) - imag(X((N+1)-kk))*Br(kk+1);

end

% 	 * Gr(N) = Xr(0) - Xi(0)
% 	 * Gi(N) = 0
% 
% Gr(N+1) = Gr(1);
% Gi(N+1) = -Gi(1);
% 
% % 	 * Symmetric part of the fft
% % 	 * Gr(2N-k) = Gr(k)
% % 	 * Gi(2N-k) = Gi(k)
% for kk=1:N-1
%     Gr(2*N-kk+1) = Gr(kk+1);
%     Gi(2*N-kk+1) = -Gi(kk+1);
% end

%X = Gr + 1j*Gi;
%X = Gr;


figure('name','reconstructed')
subplot(311)
plot(signal,'color','b')
subplot(312)
plot(Gr(1:N),'color','b');
subplot(313)
plot(Gi(1:N),'color','b');

