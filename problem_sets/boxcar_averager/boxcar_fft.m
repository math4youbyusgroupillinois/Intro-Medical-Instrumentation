function []=boxcar_fft();
% function []=boxcar_fft();
%
% Compute the FFT of the boxcar averager with a different number of samples
%
% INPUTS: None
%
% OUTPUTS: None
%
% EXAMPLE: boxcar_fft
%
% Mark Palmeri (mlp6)
% BME154L S12 PS6
% 2012-03-19

% define the number of samples for the boxcar averager
N=[1 5 10 20 50 100];

% define the sampling frequency to be able to compute the frequency axis for the FFT
fs = 1000; % Hz

for n=1:length(N),
    box = zeros(300,1);
    box(100:100+N(n)-1) = 1;
    boxcarFFT = fft(box);
    f = linspace(-fs/2,fs/2,length(boxcarFFT)); 
    subplot(length(N),2,n*2-1);
    stem(box,'MarkerSize',4);
    xlim([90 210]);
    title(sprintf('Boxcar Length (%i Samples)',N(n)));
    set(gca,'XTick',[])
    xlabel('Samples');
    if(n == 2),
        ylabel('Arbitrary Value');
    end;
    subplot(length(N),2,n*2);
    plot(f,fftshift(20*log10(abs(boxcarFFT)./max(abs(boxcarFFT)))));
    ylim([-40 0]);
    xlabel('Frequency (Hz)');
    if(n == 2),
        ylabel('Normalized Power Spectrum (dB)');
    end;
    title(sprintf('FFT (%i Samples)',N(n)));
end;

print('-depsc2','boxcar_fft.eps');
