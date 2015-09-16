

% 
% int interpret24bitAsInt32(byte[] byteArray) {     
%  int newInt = (  
%      ((0xFF & byteArray[0]) << 16) |  
%      ((0xFF & byteArray[1]) << 8) |   
%      (0xFF & byteArray[2])  
%    );  
%  if ((newInt & 0x00800000) > 0) {  
%    newInt |= 0xFF000000;  
%  } else {  
%    newInt &= 0x00FFFFFF;  
%  }  
% return newInt;  
% }  

%
% Function that interpret a 24 bits integer as a 32 bits integer. Two's complement encoding
%
function [int32_data] = int24_to_int32(uint24_data)

% cast and order the bytes
msb_byte_a = uint8(0);
msb_byte_b = uint8(uint24_data(1));
msb_byte_c = uint8(uint24_data(2));
msb_byte_d = uint8(uint24_data(3));

% check if it is a negative number
if bitand(uint8(msb_byte_b),uint8(128),'uint8')
    % extend the sign bit, we are dealing with two's complement
    msb_byte_a = bitor(msb_byte_a,uint8(255),'uint8');
end

% build the 32 bits integer
int32_data = swapbytes(typecast([msb_byte_a msb_byte_b msb_byte_c msb_byte_d],'int32'));

end

