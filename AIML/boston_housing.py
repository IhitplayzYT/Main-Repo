import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.datasets import fetch_california_housing

# Load data
X, y = fetch_california_housing(return_X_y=True)
y = y.reshape(-1, 1)

# Split
X_train, X_test, Y_train, Y_test = train_test_split(
    X, y, test_size=0.2, random_state=42
)

# Normalize (train stats only)
mu = X_train.mean(axis=0)
sig = X_train.std(axis=0)

X_train = (X_train - mu) / sig
X_test  = (X_test  - mu) / sig

# Add bias
X_train = np.c_[np.ones((X_train.shape[0], 1)), X_train]
X_test  = np.c_[np.ones((X_test.shape[0], 1)), X_test]

# Gradient Descent
n = X_train.shape[0]
M = np.zeros((X_train.shape[1], 1))

EPOCH = 10000
ALPHA = 0.01
LAMBDA = 0.01
for i in range(EPOCH):
    y_err = Y_train - X_train @ M
    loss = (1 / (2 * n)) * np.sum(y_err ** 2) + (LAMBDA/2) * (np.sum(M[1:] ** 2))
    grad = -(X_train.T @ y_err) / n
    grad[1:] += ALPHA * M[1:]
    M -= ALPHA * grad
    if i % 100 == 0:
        print(f"Iter {i}: Loss = {loss:.4f}")


    print(f"RR = {1 - (np.sum((Y_test - X_test @ M)**2)/np.sum((Y_test - Y_test.mean())**2))}")



