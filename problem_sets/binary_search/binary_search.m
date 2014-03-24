function [bin_num]=binary_search(num_base10,num_bits,base10min,base10max)
% function [bin_num]=binary_search(num_base10,num_bits,base10min,base10max)
% 
% Demonstrate how binary search works for BME354L (Spring 2013)
%
% INPUTS:
%   num_base10 (float) - base10 number to approximate
%   num_bits (int) - number of bits to use in the binary approximation
%   base10max (float) - maximum value of base10 number input
%   base10min (float) - minimum value of base10 number input
%
% OUTPUTS:
%   bin_num (Boolean vector) - binary number vector
%   Plots of the successive approximations are also generated when this runs.
%
% EXAMPLE: [bin_num] = binary_search(3.5,4,0,10);
%
% Mark Palmeri
% mlp6@duke.edu
% 2013-03-04

% pre-allocate my binary number as a logical
bin_num = logical(zeros(1,num_bits));

% compute what each LSB increment represents relative to the dynamic range of the potential input
lsb_val = (base10max - base10min)/(2^num_bits - 1)

% iterate over the number of bits
for i=1:num_bits,
    bin_num(i) = 1;
    base10guess = base10min+lsb_val*bin2dec(num2str(bin_num));
    disp(sprintf('The base-10 binary approximation is %f (base-2: %s)',base10guess,num2str(bin_num)))
    if (num_base10 > base10guess),
        bin_num(i) = 1;
        disp('  The approximation is too low; keeping this bit high.')
    else
        bin_num(i) = 0;
        disp('  The approximation is too high; setting this bit low.')
    end;
end;

disp(sprintf('\nThe final base-10 binary approximation of %f is %f (base-2: %s)',num_base10,base10guess,num2str(bin_num)))
