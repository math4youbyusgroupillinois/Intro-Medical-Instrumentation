function square_fs

n = 1:2:35;
harm = 4./(n*pi);
stem(n,harm)

t = -3:1e-2:3;
y = zeros(size(t));
y_results = [];
harm_results = [];
for c = 1:length(harm)
    harmonic = harm(c)*sin(n(c)*2*pi*t);
    y = y + harmonic;
    plot(t,harmonic,t,y);
    if n(c)==1 | n(c) == 5 | n(c) == 35 
        y_results = [y_results; y];
        harm_results = [harm_results; harmonic];
    end
end

fontname = 'times';
fontsize = 14;

clf
subplot(4,1,1)
stem(n,harm)
tstring = ['Fourier series coefficients for a square wave'];
title(tstring,'Fontname',fontname,'FontSize',fontsize);
set(gca,'Fontname',fontname,'FontSize',fontsize-2);

subplot(4,1,2)
plot(t, y_results(1,:))
hold on
plot(t, harm_results(1,:))
title(['Fundamental only, b_1 = ' num2str(harm(1))],...
    'Fontname',fontname,'FontSize',fontsize);
set(gca,'Fontname',fontname,'FontSize',fontsize-2);

subplot(4,1,3)
plot(t, y_results(2,:))
hold on
plot(t, harm_results(2,:))
tstring = ['Sum of harmonics n = 1,3,5 with n = 5 shown, b_5 = ' num2str(harm(3))];
title('Sum of harmonics n = 1,3,5, with n = 5 shown',...
    'Fontname',fontname,'FontSize',fontsize);
title(tstring,'Fontname',fontname,'FontSize',fontsize);
set(gca,'Fontname',fontname,'FontSize',fontsize-2);


subplot(4,1,4)
plot(t, y_results(3,:))
hold on
plot(t, harm_results(3,:))
tstring = ['Sum of harmonics n = 1,3,...35 with n = 35 shown, b_{35} = ' num2str(harm(18))];
title(tstring,'Fontname',fontname,'FontSize',fontsize);
set(gca,'Fontname',fontname,'FontSize',fontsize-2);

orient tall
print -deps square_fs