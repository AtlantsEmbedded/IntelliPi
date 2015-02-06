function [ SinPart CosPart ] = CreateGaborKernel( CenterFreq, BandWidth, SamplingFreq )
%CREATEGABORKERNEL Create the sin and cos Gabor Kernels
%  

SamplingPeriod = 1/SamplingFreq;
t = -5/BandWidth:SamplingPeriod:5/BandWidth;

G = exp(-t.^2/(2*(1/BandWidth)^2/(2*pi)));

SinPart = G.*sin(t*CenterFreq*2*pi);
CosPart = G.*cos(t*CenterFreq*2*pi);

CosPart = CosPart - mean(CosPart)*G/mean(G);

end

