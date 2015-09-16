function [ freq, bandwidth ] = get_freq_band_info( freq_band )


switch(freq_band)
    
    case 'delta'
        freq = 2.35;
        bandwidth = 1.65;
    case 'theta'
        freq = 5.5;
        bandwidth = 1.5;
    case 'alpha'
        freq = 11.5;
        bandwidth = 3.5;
    case 'beta'
        freq = 23.5;
        bandwidth = 7.5;
    case 'gamma'
        freq = 42.5;
        bandwidth = 17.5;
    case 'mu'
        freq = 10;
        bandwidth = 2;
end

end

