import matplotlib.pyplot as plt

X = []
f = open ("soumenDataX","r")
for line in f:
    X.append(line)
f.close()

Y = []
f = open ("soumenDataY","r")
for line in f:
    Y.append(line)
f.close()

Z = []
for i in range(20):
    Z.append(0.15736)


plt.plot(X,Y)
plt.plot(X,Z)
plt.show()
