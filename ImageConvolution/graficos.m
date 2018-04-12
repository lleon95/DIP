t_pixels =[48400 98304 173280 262144 302570 480000 518400 907148 960000 2073600];
kernel_size= [3 9 27 49 81 243 399 511 729 1023];

t_pixels=t_pixels ./1000;

res= load("resultados.csv");

#Orden de las tablas en el archivo Resultados
#Cada 11 filas resultados de un filtro diferente
#Orden de los resultados
#Separable lineal en el espacio
#No Separable lineal en el espacio
#Lineal en la frecuencia
#Gaussiano en el espacio
#Gaussiano en la frecuencia
#

SLS= res(2:11,2:11);
NSLS= res(13:22,2:11);
LF= res(24:33,2:11);
GS= res(35:44,2:11);
GF=res(46:55,2:11);

figure(1);
colormap(hot);
mesh(t_pixels,kernel_size,NSLS);

xlabel("Cantidad de pixeles (Kilo)");
ylabel ("Tamaño de kernel");
zlabel ("tiempo (segundos)");
hidden('off');
title("Resultados de Filtro No Separable en el espacio")


figure(2);
colormap(jet);
mesh(t_pixels,kernel_size,SLS);
xlabel("Cantidad de pixeles (Kilo)");
ylabel ("Tamaño de kernel");
zlabel ("tiempo (segundos)");
hidden('off');
title("Resultados de Filtro Separable en el Espacio")

figure(3);
colormap(gray);
mesh(t_pixels,kernel_size,LF);
xlabel("Cantidad de pixeles (Kilo)");
ylabel ("Tamaño de kernel");
zlabel ("Tiempo (segundos)");
hidden('off');
title("Resultados de Filtro Lineal en la Frecuencia")

figure(4);
# Linear - Frequency
h1 = mesh(t_pixels,kernel_size,LF, "facecolor", "none", "edgecolor", "b");
hold on;
# Non Separable Linear - Space
h2 = mesh(t_pixels,kernel_size,NSLS, "facecolor", "none", "edgecolor", "g");
hold on;
# Separable Linear - Space
h3 = mesh(t_pixels,kernel_size,SLS, "facecolor", "none", "edgecolor", "r");
# Legend
legend([h1,h2,h3], {"Lineal - Frecuencia", "Lineal No Separable - Espacio", "Linea Separable - Espacio"});
xlabel("Cantidad de pixeles (Kilo)");
ylabel ("Tamaño de kernel");
zlabel ("tiempo (segundos)");
hidden('off');
title("Resultados de Filtro Lineal en espacio y frecuencia");

figure(5);
# Gaussiano - Space
g1 = mesh(t_pixels,kernel_size,GS, "facecolor", "none", "edgecolor", "b");
hold on;
# Gaussian - Frequency
g2 = mesh(t_pixels,kernel_size,GF, "facecolor", "none", "edgecolor", "r");
# Legend
legend([g1,g2], {"Gaussiano - Espacio", "Gaussiano - Frecuencia"});
xlabel("Cantidad de pixeles (Kilo)");
ylabel ("Tamaño de kernel");
zlabel ("tiempo (segundos)");
hidden('off');
title("Resultados de filtro Gaussiano en espacio y frecuencia");

