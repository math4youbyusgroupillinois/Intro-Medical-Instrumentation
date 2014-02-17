figure;
y = [-60 20 20];
x = [10 1000 100000];
semilogx(x,y,'LineWidth',3);
xlabel('f (Hz)','FontSize',18);
ylabel('|H(jf)| (dB)','FontSize',18);
axis([10 100000 -70 30]);
title('Bode Magnitude Plot','FontSize',18);
set(gca,'XTick',[100 1000 10000 100000]);
set(gca,'XMinorTick','Off');
set(gca,'FontSize',18);
set(gca,'XGrid','on');
set(gca,'XMinorGrid','off');
set(gca,'YGrid','on');

print -deps2 bode_plot.eps
