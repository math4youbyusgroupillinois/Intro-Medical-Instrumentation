v2(1:120) = 0;
v2(10:60) = -5;

v1(1:120) = 0;
v1(20:29) = 2;
v1(30:39) = -5;
v1(50:59) = -5;
v1(60:69) = 2;
v1(80:89) = -5;
v1(90:99) = 2;

t = (1:120);
figure;
subplot(2,1,1);
plot(t,v1,'LineWidth',3);
xlabel('Time (s)','FontSize',18);
ylabel('V1 (V)','FontSize',18);
axis([0 120 -6 3]);
set(gca,'FontSize',18);
set(gca,'YTick',[-5 0 2]);

subplot(2,1,2);
plot(t,v2,'LineWidth',3);
xlabel('Time (s)','FontSize',18);
ylabel('V2 (V)','FontSize',18);
axis([0 120 -6 1]);
set(gca,'FontSize',18);
set(gca,'YTick',[-5 0]);

print -deps2 sum_comparator_v1_v2.eps
