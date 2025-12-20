#@ Imports
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
#@ Loading dataset
data = pd.read_csv("~/csv/archive/train.csv");
data = data.dropna()
#@ Cleaning and formatting data
X = np.array(data["x"]).reshape(-1,1)
Y = np.array(data["y"]).reshape(-1,1)
X = np.c_[np.ones((X.shape[0],1)),X]
M = np.zeros((X.shape[1],1))
EPOCH = 1000
ALPHA = 0.00005
for i in range(EPOCH):
    y_err = Y - (np.dot(X,M))
    grad = -(2/X.shape[0]) * (X.T @ y_err)
    M = M - ALPHA * grad
    if i % 10 == 0 :
        print("Loss in iter "+ str(i) +" : " + str(np.sum(y_err ** 2)/X.shape[0]))

print(M)
#@ Test data
test_data = pd.read_csv("~/csv/archive/test.csv").dropna()
print(test_data)
X_test = np.array(test_data["x"]).reshape(-1,1)
Y_test = np.array(test_data["y"]).reshape(-1,1)
X_test = np.c_[np.ones((X_test.shape[0],1)),X_test]
print(f"{X_test.shape} {Y_test.shape}")

Y_pred = X_test @ M
mse = np.mean((Y_test - Y_pred) ** 2)
print("Test MSE:", mse)
ss_res = np.sum((Y_test - Y_pred) ** 2)
ss_tot = np.sum((Y_test - Y_test.mean()) ** 2)
r2 = 1 - ss_res / ss_tot

print("R²:", r2)




