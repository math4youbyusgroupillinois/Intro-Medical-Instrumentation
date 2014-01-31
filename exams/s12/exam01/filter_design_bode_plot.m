Hmag = [10 10 -50 30 30];
Hw = [1e0 1e3 1e6 1e8 1e10];

figure;
semilogx(Hw,Hmag,'LineWidth',3);
xlabel('\omega (rad/s)','FontSize',18);
ylabel('|H(\omega)| (dB)','FontSize',18);
axis([0 1e10 -60 40]);
set(gca,'FontSize',18);
set(gca,'XTick',Hw);
%set(gca,'XTickLabel',{'1e0','1e3','1e6','10^8','1e10'},'Interpreter','latex');
set(gca,'YTick',[-50 10 30]);
grid on

print -deps2 filter_design_bode_plot.eps
