v1(1:100) = -5;
v1(20:39) = 5;
v1(40:59) = 2;
v1(60:69) = 2;

t = (1:100);
figure;
plot(t,v1,'LineWidth',3);
xlabel('Time (s)','FontSize',18);
ylabel('V1 (V)','FontSize',18);
axis([0 100 -6 6]);
set(gca,'FontSize',18);
set(gca,'YTick',[-5 0 2 5]);

print -deps2 comparator_v1.eps
