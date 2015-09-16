function [ sin_part cos_part ] = create_gabor_filter( center_freq, bandwidth, sampling_freq )


SamplingPeriod = 1/sampling_freq;
t = -5/bandwidth:SamplingPeriod:5/bandwidth;

G = exp(-t.^2/(2*(1/bandwidth)^2/(2*pi)));

sin_part = G.*sin(t*center_freq*2*pi);
cos_part = G.*cos(t*center_freq*2*pi);

cos_part = cos_part - mean(cos_part)*G/mean(G);

end

