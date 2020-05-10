% Octave can handle doulbe quotes "". In addition the load() command
% is able to treat the header correctly (in contrast to MATLAB)

a = load("data")

plot(a(:,1), a(:,2), 'r-o')
grid()

title("Simulated inductance L in dependency of the distance d between coil and outer boundary")

xlabel("d(m)")
ylabel("L(H)")

legend("Inductance L")

% Use the print() command to export a graph
print("plot.png")

