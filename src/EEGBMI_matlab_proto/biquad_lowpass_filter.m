function [ filtered_signal ] = biquad_lowpass_filter( signal, show_filter )


if nargin<2
   show_filter = 0; 
end

T = 1;
Q = 0.707;
fc = 2*0.2727;
fc = 0.8;
Bw = fc/Q;

R = exp(-pi*Bw*T);


% calculated with: http://www.earlevel.com/main/2010/12/20/biquad-calculator/
a0 = 0.33597142261668933;
a1 = 0.6719428452333787;
a2 = 0.33597142261668933;
b1 = 0.1674257156004825;
b2 = 0.1764599748662749;


B = [b2 b1];
A = [a2 a1 a0];

if show_filter
    [h, w] = freqz(A,B);

    N = length(h);
    plot(w/pi, h); grid;
end
 
filtered_signal = filter(A,B,signal);

end

