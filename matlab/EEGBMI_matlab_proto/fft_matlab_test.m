

% create a signal with known properties
xx = (0:2^12-1)*0.001; % resolution of 0.01 seconds

signal = sin(xx*2*pi*4)+2*sin(xx*2*pi*7);

plot(signal)

figure;
subplot(211)
result = fft_matlab(signal,2^12);
plot(2*abs(result))


subplot(212)
result = fft(signal,2^12);
plot(2*abs(result))